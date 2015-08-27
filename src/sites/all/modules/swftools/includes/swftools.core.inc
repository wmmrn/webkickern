<?php 

/**
 * @file
 * Implements SWF Tools core functions that are common to the main module and the API module.
 * 
 * Core functions in this file are needed by both the lightweight API, and the
 * full SWF Tools module.
 * 
 * Core functions allow SWF Tools to:
 * - Retrieve a list of embedding methods from enabled modules
 * - Report the direct embedding method as available
 * - Theme content using the swftools_direct embedding method
 * - Theme elements of #type => 'swftools'
 * - Return constants defining the library, alternate content,
 *   and the default for adding JavaScript to all pages.
 */

/**
 * @addtogroup swftools
 * @{
 */

/**
 * JavaScript will be added to all pages when they are served, even if SWF Tools isn't called directly.
 */
define('SWFTOOLS_ALWAYS_ADD_JS', 0x0001);

/**
 * The default alternate markup string that is displayed if the embedding process fails.
 */
define('SWFTOOLS_DEFAULT_HTML_ALT', '<p>You are missing some Flash content that should appear here! Perhaps your browser cannot display it, or maybe it did not initialize correctly.</p>');

/**
 * JavaScript will be added inline with the page markup.
 */
define('SWFTOOLS_JAVASCRIPT_INLINE', 0x0000);

/**
 * JavaScript will be added to the page header.
 */
define('SWFTOOLS_JAVASCRIPT_HEADER', 0x0001);

/**
 * JavaScript will be added to the page footer.
 */
define('SWFTOOLS_JAVASCRIPT_FOOTER', 0x0002);

/**
 * Turns an SWF Tools data array in to markup for inclusion on the page, 
 * using W3C compliant HTML.
 * 
 * @param string $file
 *   The path to the file to be displayed.
 * @param array $data
 *   An array of additional parameters associated with this item.
 *   
 * @return string
 *   A markup string.
 *   
 * @ingroup themeable
 */
function theme_swftools_direct($file, $data) {
  
  // Strip out unset values, e.g. bgcolor
  $data['params'] = array_filter($data['params']);
  
  // Build parameters string
  $params = '';
  foreach ($data['params'] as $key => $value) {
    $params .= '<param name="' . check_plain($key) . '" value="' . check_plain($value) . '" />';
  }
  
  // Attach flashvars parameter, if flashvars are present
  // drupal_query_string_encode() implodes with & which breaks XHTML validation. See #1011402.
  //  $params .= $data['flashvars'] ? '<param name="flashvars" value="' . drupal_query_string_encode($data['flashvars']) . '" />' : '';
  $params .= $data['flashvars'] ? '<param name="flashvars" value="' . swftools_query_string_encode($data['flashvars']) . '" />' : '';

  // Construct embedding markup
  $html  = '<object id="' . $data['othervars']['id'] . '" name="' . $data['othervars']['id'] . '" classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="' . $data['othervars']['width'] . '" height="' . $data['othervars']['height'] . '">';
  $html .= '<param name="movie" value="' . $file . '" />';
  $html .= $params;
  $html .= '<!--[if gte IE 7]>-->';
  $html .= '<object name="' . $data['othervars']['id'] . '" type="application/x-shockwave-flash" data="' . $file . '" width="' . $data['othervars']['width'] . '" height="' . $data['othervars']['height'] . '">';
  $html .= $params;
  $html .= '<!--<![endif]-->';
  $html .= theme('swftools_html_alt', $data);
  $html .= '<!--[if gte IE 7]>-->';
  $html .= '</object>';
  $html .= '<!--<![endif]-->';
  $html .= '</object>';
  
  // Return the result
  return $html;

}


/**
 * Themes an swftools element by passing it on to the appropriate theme function.
 * 
 * This theme function implements the swftools Form API element.
 * 
 * If the incoming element is not an array someone is calling to place a simple
 * swf on the page and they're using the API, so hand it to theme_swftools_api()
 * 
 * If the incoming element is an array, and SWF Tools main module is present then
 * hand the element to swf() as it provides more power, such as auto-detection of
 * sizes and automatic rendering of media in to players.
 * 
 * If the incoming element is an array, and SWF Tools main module is not avaiable
 * then assume we have a simple swf, so call theme_swftools_api() with just
 * $element['#value']
 * 
 * @param mixed $content
 *   A string, or an array, that defines the SWF Tools output required.
 * @param string $options
 *   Optional array of other data to pass to SWF Tools.
 *   
 * @return string
 *   Markup to render the object.
 *
 * @ingroup themeable
 */
function theme_swftools($content, $options = array()) {

  // If the incoming element is a Forms API element remap it  
  if (is_array($content) && isset($content['#value'])) {
    $options = array(
      'params' => $content['#params'],
      'othervars' => $content['#othervars'],
      'flashvars' => $content['#flashvars'],
      'methods' => $content['#methods'],
    );
    $content = $content['#value'];
  }
  
  // Is the full version of SWF Tools installed?
  if (defined('SWFTOOLS_INSTALLED')) {

    // Hand over to swf() and return the result
    return $content ? swf($content, $options) : '';
    
  }
  
  // Hand over to theme_swftools_api and return the result
  return $content ? theme('swftools_api', $content, $options) : '';

}


/**
 * Themes the alternate HTML markup for an SWF Tools element.
 * 
 * A theme is used here so that the site can over-ride and modify
 * the html_alt output before it is placed on the page. This allows
 * more complex output than the fixed HTML alternate that can be
 * specified on the settings page.
 * 
 * The SWF Tools data array is passed to the theme so that the
 * themer has access to all the data just prior to output. It can
 * use this to modify its results accordingly.
 * 
 * @param array $data
 *   The SWF Tools data array.
 *   
 * @return string
 *   A string of markup.
 *   
 * @ingroup themeable
 */
function theme_swftools_html_alt($data) {

  // If html_alt is empty then populate it with default text
  // TODO: What if the user wants an empty string?
  $data['othervars']['html_alt'] = $data['othervars']['html_alt'] ? $data['othervars']['html_alt'] : variable_get('swftools_html_alt', SWFTOOLS_DEFAULT_HTML_ALT);
  
  // Return resulting string after filtering it
  return check_markup($data['othervars']['html_alt'], variable_get('swftools_html_alt_format', FILTER_FORMAT_DEFAULT), FALSE);

}


/**
 * @} End of "addtogroup swftools"
 */


/**
 * Verifies that the supplied id is unique, and assigns an id if one was not supplied.
 * 
 * Typically this is called to get us a unique id, but the user can also supply an id. In all cases
 * we call form_clean_id to verify that the id is unique in this page generation session.
 * 
 * @param string $id
 *   An id to be checked, or empty if we don't have one yet.
 * @param bool $clean_id
 *   When TRUE the id will be checked to make sure that it is unique.
 *   When FALSE the id will not be checked for uniqueness so the user can be sure
 *   the id returned has not been modified unexpectedly. The called is responsible
 *   for making sure ids are unique on the page.
 *   
 * @return string
 *   A form id.
 */
function swftools_get_id($id = '', $clean_id = TRUE) {
  
  // If a specific id wasn't set then assign one
  $id = $id ? $id : uniqid('swftools-');
  
  // Return the id, checking that is unique (clean) if required
  return $clean_id ? form_clean_id($id) : $id;
 
}


/**
 * Flushes all caches when new settings are stored.
 * 
 * This function is called by player and embedding modules as a
 * submit handler to ensure all cached content is purged.
 */
function swftools_admin_settings_submit($form, &$form_state) {
  
  // Flush all caches so new players appear
  drupal_flush_all_caches();
    
}


/**
 * Implementation of hook_swftools_methods().
 * 
 * Note that we use a trick here as this method is always available.
 * We implement system_swftools_methods() as we know the system module
 * will always be present so we can be sure this gets reported.
 */
function system_swftools_methods() {
  
  $methods['swftools_embed_method']['swftools_direct'] = array(
    'module'       => 'swftools',
    'title'        => t('Direct embedding'),
  );

  return $methods;

}


/**
 * Collects information from all modules about the players and embedding methods available.
 * 
 * @param string $action
 *   Optional parameter to retrieve data only about a specific method.
 * @param bool $reset
 *   Optional parameter which if TRUE will reset the method cache and rebuild it.
 *   
 * @return array
 *   An array of data describing the available methods.
 */
function swftools_get_methods($action = NULL, $reset = FALSE) {

  // Cache methods array as it may be called several times
  static $methods;

  // If user has requested the cache to be reset then reset it
  if ($reset || !isset($methods)) {
    if (!$reset && ($cached = cache_get('swftools:methods'))) {
      $methods = $cached->data;
    }
    else {
      $methods = array();
      foreach (module_implements('swftools_methods') as $module) {
        $function = $module . '_swftools_methods';
        if (function_exists($function)) {
          $implements = $function();
          foreach ($implements as $method_type => $method) {
            foreach ($method as $method_name => $method_details) {
              $method_details += array(
                'name'     => $method_name,
                'module'   => '',
                'title'    => '',
                'download' => '',
                'library'  => '',
                'profile'  => array(),
              );
              if ($method_type == 'swftools_embed_method') {
                $method_details += array(
                  'theme' => $method_name,     
                );
              } 
              else {
                $method_details += array(
                  'version'  => 7,
                );
              }             
              $methods[$method_type][$method_name] = $method_details;
            }
          }
        }
      }
      cache_set('swftools:methods', $methods);
    }
  }
  
  // In case no module is presenting a method for the required action the following line avoids a notice error
  if ($action) {
    $methods += array(
      $action => NULL,
    );
  }
  
  // Return results - either for the specific action, or the whole array
  return $action ? $methods[$action] : $methods;

}


/**
 * Returns a path to the requested library, using Libraries API when it is available.
 * 
 * If the Libraries API is present then the library will be searched for under the
 * profiles/$profile/libraries, sites/all/libraries, and sites/sitename/libraries.
 * 
 * sites/sitename/libraries has the highest priority and will "win" if it is present.
 * 
 * If Libraries API is not present then the library should be present in
 * sites/all/libraries
 * 
 * @param string $library
 *   The name of the library, e.g. jwplayer4.
 *   
 * @return string
 *   A path to the requested library.
 * 
 * @see http://drupal.org/project/libraries Libraries API
 */
function swftools_get_library($library) {

  return ($ret = module_invoke('libraries', 'get_path', $library)) ? $ret : 'sites/all/libraries/' . $library;
  
}


/**
 * Initialise the SWF Tools options array with some standard elements.
 * 
 * To avoid NOTICE errors later we must make sure that some basic arrays
 * are present.
 * 
 * @param string $options
 *   Options array to initialise.
 *   
 * @return nothing
 *   Manipulate the array by reference.
 */
function swftools_initialise_options(&$options) {
  
  // Initialise any array elements that weren't passed by the caller
  $options += array(
    'params' => array(),
    'flashvars' => array(),
    'othervars' => array(),
    'methods' => array(),
  );
  
  // Initialise othervars with some defaults
  $options['othervars'] += array(
    'id' => '',
    'html_alt' => '',
  );
  
}


/**
 * Customised version of drupal_query_string_encode().
 * 
 * drupal_query_string_encode() implodes using &, which means a Flashvars string
 * appears to fail XHTML validation. This version implode using &amp; which will
 * pass XHMTL validation.
 * 
 * http://jonahchanticleer.com/archives.cfm/category/from-red-to-green/page/1
 * 
 * Issue originally raised in #1011402
 * 
 * @param array $query
 * @param array $exclude
 * @param string $parent
 * 
 * @return string
 */
function swftools_query_string_encode($query, $exclude = array(), $parent = '') {
  $params = array();

  foreach ($query as $key => $value) {
    $key = rawurlencode($key);
    if ($parent) {
      $key = $parent .'['. $key .']';
    }

    if (in_array($key, $exclude)) {
      continue;
    }

    if (is_array($value)) {
      $params[] = swftools_query_string_encode($value, $exclude, $key);
    }
    else {
      $params[] = $key .'='. rawurlencode($value);
    }
  }

  return implode('&amp;', $params);
}
