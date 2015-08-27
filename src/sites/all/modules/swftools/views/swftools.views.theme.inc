<?php

/**
 * @file
 * Functions used as part of theming of content by the SWF Tools Views plug-in.
 */

/**
 * Template preprocessor for views-view-views-rss-fields.tpl.php.
 */
function template_preprocess_views_view_swftools(&$vars) {
  
  // Initialise swftools options array
  $vars['swftools']['options'] = array(
    'othervars' => array(),
    'flashvars' => array(),
    'methods' => array(),
    'params' => array(),
  );

  // Assign the relevant profile
  if ($vars['options']['profile'] != SWFTOOLS_UNDEFINED) {
    $vars['swftools']['options']['othervars']['profile'] = $vars['options']['profile'];
  }
  
}

/**
 * Adds a single element to the playlist.
 * 
 * This function is designed to be called by templates and it will take care of
 * adding the fileurl, image and title to the playlist. These are the only
 * defaults that SWF Tools takes care of by itself. For more advanced playlists
 * it is required to create a customised views-view-swftools.tpl.php
 * 
 * @param array $files
 *   The playlist array (passed by reference).
 * @param int $key
 *   The key in the array for this playlist element.
 * @param array $options
 *   The options variable array for the view.
 * @param array $work
 *   The results of rendering this row, keyed on field id.
 *   
 * @return nothing
 *   The $files array is passed by reference.
 */
function swftools_views_add_playlist_element(&$files, $key, $options, $work) {
  
  // Always attach the filepath
  $files[$key]['filepath'] = $work[$options['filepath']];
  
  // Attach other keys, using empty string if they are undefined
  $elements = array(
    'title',
    'image',
    'description',
    'author',
    'date',
    'link',
    'duration',
  );

  foreach ($elements as $element) {
    $files[$key][$element] = ($options[$element] != SWFTOOLS_UNDEFINED) && $work[$options[$element]] ? $work[$options[$element]] : '';
  }
    
}
