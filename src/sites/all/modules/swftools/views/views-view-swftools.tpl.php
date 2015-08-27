<?php

/**
 * @file
 * Template for generating a Views based playlist and player.
 *
 * We iterate in here and not the process function because we want the user to
 * be able to modify this template easily. The code below builds a playlist
 * ready to go SWF Tools. We don't do it in template_preprocess_views_view_swftools()
 * because if the you are supplying arbitrary data to pass on to a playlist then
 * we don't want to have to iterate again. Note that the code below includes a
 * call to a function that will automatically take care of assigning the fileurl,
 * the image thumbnail and a title if these are set on the options form.
 * 
 * You can then add additional data to the playlist array as you wish, picking up
 * other fields that have been fetched out of Views. For example, you can fetch
 * the node body and attach it on an element called 'description' and then that
 * can be used by TiltViewer to populate the reverse of the images.
 */

// Get the fields we are using
$keys = array_keys($view->field);

// Initialise a playlist ready for SWF Tools
$files = array();

//  Iterate over each row of the results - this is a clever bit of processing to match files with their thumbnails
foreach ($rows as $key => $row) {
  
  // Reset the work array
  $work = array();
    
  // Render each field in this row using the assigned handler
  foreach ($keys as $id) {
    $work[$id] = $view->field[$id]->theme($row);
  }

  // Add this item to the playlist
  swftools_views_add_playlist_element($files, $key, $options, $work);
    
  /**
   * If you want to add arbitrary data to the playlist then you can do so here
   * Add your additional elements to $files[$key]['myKey'] = myValue
   * Rendered fields are available in $work[$id] where $id is the field name.
   * 
   * See swftools.views.theme.inc to see mappings available by default
   */
  
}

// Call swf to output the result
print swf($files, $swftools['options']);

?>
