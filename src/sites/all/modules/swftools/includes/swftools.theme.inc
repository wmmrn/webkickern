<?php

/**
 * @file
 * Implements SWF Tools theme functions.
 * 
 * As the number of theme functions has grown it seemed it was time to collect
 * them all up in a separate file.
 */

/**
 * @addtogroup swftools
 * @{
 */

/**
 * Produces finished markup ready for inserting on the page.
 * 
 * This function hands off to the appropriate embedding theme first, and then
 * wraps the result in some wrapper divs before returning the result.
 *
 * @param array $data
 *   An SWF Tools array of data.
 *   
 * @return string
 *   An HTML string that generates the output
 *   
 * @ingroup themeable
 */
function theme_swftools_embed($data) {
  
  // Call the specific embedding function - this generates the mark to render the swf
  // The default is a theme with the same name as the method, but this can be over-ridden by modifying the theme key
  
  /**
   * Note that the theme functions expect to receive the fileurl as a separate parameter to the
   * data array. Strictly we could just pass the data array, but if we rewrite the theme
   * functions then it will break other modules that are calling using separate file and data
   * array. For now (SWF Tools 6) leave this as it is, but for SWF Tools 7 we will simply pass
   */
  $embed_markup = theme($data['resolved_methods']['embed']['theme'], $data['othervars']['fileurl'], $data, variable_get('swftools_javascript_location', SWFTOOLS_JAVASCRIPT_INLINE));
  
  // Prepare an array of classes to include in the wrapper div
  $classes[] = 'swftools';
  $classes[] = 'swftools-' . str_replace('_', '-', $data['resolved_methods']['player']['name']);

  // If the user provided class data already then don't over-rule it
  if (!empty($data['othervars']['class'])) {
    $classes[] = $data['othervars']['class'];
  }

  // TODO: Accommodate weights on this?
  
  // Allow for handling of #prefix and #suffix supplied via othervars
  $data['othervars'] += array(
    '#prefix' => '',
    '#suffix' => '',
  );
  
  // Wrap $embed_markup with prefix and suffix (e.g. to add accessible controls)
  $embed_markup = $data['othervars']['#prefix'] . $embed_markup . $data['othervars']['#suffix'];
  
  // Return completed markup
  return '<div id="swftools-' . $data['othervars']['id'] . '" class="' . implode(' ', $classes) . '">' . $embed_markup . '</div>';

}


/**
 * Themes multiple value CCK content in to a playlist.
 * 
 * @param array $element
 *   The element to render.
 * @param array $profile
 *   (optional) Array of profile data for the profile that is being used.
 *   
 * @return string
 *   Markup to produce the flash content, or nothing if the element was empty.
 *   
 * @ingroup themeable
 */
function theme_swftools_formatter_playlist($element, $profile = array()) {

  // Initialise an array for results
  $files = array();
  
  // Get the children
  $children = element_children($element);
  
  // If there is only one child then maybe we don't want a playlist
  if (count($children) == 1) {

    // Pop the first value of the children array
    $_children = $children;
    $child = array_pop($_children);
        
    // Get the name of the alternate formatter for this content type
    $formatter_name = variable_get('swftools_' . $element['#type_name'] . '_' . $element['#field_name'], 'swftools_playlist');
    
    // What happens next depends on the formatter name
    
    switch ($formatter_name) {
      
      case 'hidden':      
    
        // If the format is set to hidden then return nothing
        if ($formatter_name == 'hidden') {
          return;
        }
        
      case 'swftools_playlist':
        
        // If swftools_playlist don't do anything different
        break;

      default:

        // Find out what the alternate formatter should be
        if ($formatter = _content_get_formatter($formatter_name, 'filefield')) {
          
          // We can work out the theme name from the formatter information
          $theme = $formatter['module'] .'_formatter_'. $formatter_name;
    
          // Construct a modified element that mimics a single element
          $element['#formatter'] = $formatter_name;
          $element['#theme'] = $theme;
          $element += $element[$child];
          $element['#item']['#delta'] = 0;
          unset($element[$child]);
          
          // Theme this new element according to the alternate formatter
          return theme($theme, $element);
          
        }
    }
  }

  // Retrieve images
  $images = theme_swftools_formatter_thumbnail('', TRUE);  
  
  // Cycle through the file elements
  foreach ($children as $key) {

    // Is this a filefield?
    if (isset($element[$key]['#item']['filepath'])) {
      $files[$key] = array(
        'filepath' => $element[$key]['#item']['filepath'],
        'title' => $element[$key]['#item']['data']['description'], 
      );
    }
    
    // Is this a link field?
    if (isset($element[$key]['#item']['url'])) {
      $files[$key] = array(
        'filepath' => $element[$key]['#item']['url'],
        'title' => $element[$key]['#item']['title'], 
      );
    }
    
    // Is this a text field?
    // TODO: How to handle streams in a playlist
    if (isset($element[$key]['#item']['value'])) {
      $files[$key] = array(
        'filepath' => $element[$key]['#item']['value'],
        'title' => '', 
      );
    }

    // Is there an image?
    if (isset($images[$key]) && $images[$key]) {
      
      // Get the path to the image
      $source = swftools_get_url_and_path($images[$key]);
      
      // If $source returned a result then use it
      if ($source) {
        $files[$key]['image'] = $source['fileurl'];
      }

    }
    
  }
  
  // If files array is empty then there is nothing to be rendered
  if (empty($files)) {
    return;
  }
  
  // Pass element to the swf tools processor by attaching it in othervars
  $data['othervars']['cck'] = $element;
  
  // Assign the profile
  $data['othervars']['profile'] = $profile ? $profile['profile'] : '';
  
  // But if we got something then we can call swf() now to render it
  return swf($files, $data);
  
}


/**
 * Theme function to store and retrieve thumbnail images.
 * 
 * If called as a regular theme function it stores the path of the image,
 * but if the retrieve flag is set it returns that path.
 * 
 * In effect this function is a local store to hold the most recently "seen"
 * image on behalf of the swf themer. In order for this code to work the
 * thumbnail theme function must come before the swf theme function, so it
 * is necessary to order the fields appropriately on the content configuration
 * page.
 * 
 * When caching thumbnails then if the element delta is zero the cache is
 * flushed first to start a new collection of images.
 * 
 * @param array $element
 *   The CCK element to store.
 * @param bool $retrieve
 *   When TRUE will return the currently stored path.
 * @param int $delta
 *   If set then attempt to retrieve just the image at the specified delta, otherwise return
 *   the whole array of stored thumbnails.
 *   
 * @return mixed
 *   A string, or an array, holding the thumbnail filepaths.
 *   
 * @ingroup themeable
 */
function theme_swftools_formatter_thumbnail($element, $retrieve = FALSE, $delta = NULL) {
  
  // Create a static variable to hold the image path
  static $image_path = array();
  
  // It retrieving a previous thumbnail then return its path and reset the stored value
  // We may want to retrieve all cached thumbnails ($delta = NULL), or a specific thumbnail
  // in which case $delta is the one we want
  if ($retrieve) {
    if ($delta !== NULL) {
      if (isset($image_path[$delta])) {
        return $image_path[$delta];
      }
      else {
        return '';
      }
    }
    else {
      return $image_path ? $image_path : '';
    }
  }
   
  // If the delta is zero we are starting a new collection so reset in case
  // a retrieval call wasn't made by the theme function
  if ($element['#item']['#delta'] == 0) {
    $image_path = array();
  }
  
  // If the element is empty set the stored path to an empty string
  if (empty($element['#item']['fid']) && empty($element['#item']['value']) && empty($element['#item']['url'])) {
    $image_path[] = '';
    return;
  }

  // Initialise an empty string
  $image_url = '';
  
  // See if we're processing a filefield and get the appropriate value
  if (isset($element['#item']['filepath'])) {
    $image_url = $element['#item']['filepath'];
  }

  // See if we're processing a link field and get the appropriate value
  elseif (isset($element['#item']['url'])) {
    $image_url = $element['#item']['url'];
    
    // add the query to the base url, if necessary
    if (isset($element['#item']['query']) && strlen($element['#item']['query']) > 0) {
      $image_url .= '?' . ($element['#item']['query']);
    }
    
    // add the fragment to the url, if necessary
    if (isset($element['#item']['fragment']) && strlen($element['#item']['fragment']) > 0) {
      $image_url .= '#' . ($element['#item']['fragment']);
    }
    
  } 
  
  // We're processing a text field so get the appropriate value
  else {
    $image_url = $element['#item']['value'];

    // Check this is a valid url
    $image_url = check_url($image_path);
    
  }

  // Store the result
  $image_path[] = $image_url;
  
  // Return nothing at this point
  return;

}


/**
 * Themes a CCK element in to flash content.
 * 
 * @param array $element
 *   The CCK element to render.
 *   
 * @return string
 *   A string of markup to produce the flash content, or nothing if the element was empty.
 *   
 * @ingroup themeable
 */
function theme_swftools_formatter_swftools($element, $profile = array()) {

  // If the element is empty return
  if (empty($element['#item']['fid']) && empty($element['#item']['value']) && empty($element['#item']['url'])) {
    return '';
  }

  // See if a thumbnail image has been stored with this content
  $image = theme_swftools_formatter_thumbnail('', TRUE, $element['#item']['#delta']);
  
  // If an image was stored then set the thumbnail
  if ($image) {
    $data['othervars']['image'] = $image;
  }
  else {
    $data = array();
  }

  // Assign the profile
  $data['othervars']['profile'] = $profile ? $profile['profile'] : '';
  
  // See if we're processing a filefield and get the path and description
  if (isset($element['#item']['filepath'])) {
    $swf = $element['#item']['filepath'];
    $data['othervars']['title'] = $element['#item']['data']['description'];
    $source = 'filefield';
  }

  // See if we're processing a link field and get the path and title
  elseif (isset($element['#item']['url'])) {
    $swf = $element['#item']['url'];
    $data['othervars']['title'] = $element['#item']['title'];
    $source = 'link';
    
    // add the query to the base url, if necessary
    if (isset($element['#item']['query']) && strlen($element['#item']['query']) > 0) {
      $swf .= '?' . ($element['#item']['query']);
    }
    
    // add the fragment to the url, if necessary
    if (isset($element['#item']['fragment']) && strlen($element['#item']['fragment']) > 0) {
      $swf .= '#' . ($element['#item']['fragment']);
    }
  } 
  
  // We're processing a text field so get the path
  else {
    $swf = $element['#item']['value'];
    $data['othervars']['title'] = '';
    $source = 'text';

    // We need to run $swf through check_url to make sure it is safe, but rtmp isn't in the allowed protocols
    // This is configurable (see filter.module line 1182) but not sure if the variable is exposed
    // $swf = check_url($swf);
    
    // See if we have been given a stream by trying to explode the string
    $stream = explode(' ', $swf);
    
    // If the explode return 2 elements assume we have a stream, element[0] contains the source, element[1] the file
    if (count($stream) == 2) {
      $data['othervars']['stream'] = $stream[0];
      $swf = $stream[1];
    }
  }
  
  // Pass element to the swf tools processor by attaching it in othervars
  $data['othervars']['cck'] = $element;
  
  // Get the markup for the flash content from swf()
  $return = swf($swf, $data);
  

  
  // Add the download link if required (we are either using swftools formatter, or a profile with this option enabled)
  if ($element['#formatter'] == 'swftools' || !empty($profile['download_link'])) {
    switch ($source) {
      case 'filefield':
        $return .= "\n" . theme('filefield_formatter_default', $element);
        break;
      case 'link':
        $return .= "\n" . theme('link_formatter_default', $element);
        break;
    }
  }
  
  // Return the resulting markup
  return $return;
}


/**
 * Returns a path to a default image.
 * 
 * This is generally called by playlist functions that are rendering images as
 * part of the playlist. In some instances it is necessary to use a placeholder
 * image when none is given in order for the playlist to render correctly.
 * 
 * By over-riding this theme function alternate images can be used. The data
 * array is provided so if this theme function is over-ridden custom
 * alternatives can be used depending on the context. By default SWF Tools
 * returns a single pixel gif.
 * 
 * @param array $data
 *   The SWF Tools data array.
 *   
 * @return string
 *   Path to the default image.
 * 
 * @ingroup themeable
 */
function theme_swftools_empty_image($data) {
  return base_path() . drupal_get_path('module', 'swftools') . '/includes/empty.gif';  
}


/**
 * @} End of "addtogroup swftools"
 */
