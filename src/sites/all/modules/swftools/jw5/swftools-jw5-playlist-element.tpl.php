<?php

/**
 * @file
 * Template for a single entry in a LongTail JW Player 5 playlist.
 */

/**
 * Prepare xml output for a playlist
 * The markup is placed in a template so that it can be easily over-ridden by the theme system
 * to allow flexibility
 * 
 * This template formats single elements for the xml playlist. By modifying the content here
 * the xml can be modified directly to provide customised xml handling for different players.
 * 
 * By way of example this implementation of the template will include data from the ID3 tags
 * to pass the length of the clip, plus album and year data, if available.
 * 
 * Available variables:
 * - $element: An array of available playlist data for this element.
 * 
 * As a minimum the $element array can be expected to contain the following keys:
 * - fileurl
 *   The resolved url to the file
 * - filepath
 *   The filepath of the file (which will be different to the url if on the local file system)
 * - filename
 *   The filename of the file, obtained from the url
 * - title
 *   The title of the file, e.g. user supplied, assigned by ID3, or may be the filename
 *   
 * Other keys may be available, depending on which additional modules have been enabled. 
 * 
 */
?>

<track>

<?php

  // See if playtime is available from id3
  $duration = isset($element['getid3']['playtime_string']) ? $element['getid3']['playtime_string'] : '';
  
  // Suppress notice errors when
  $element += array(
    'author' => '',
    'description' => '',
    'image' => '',
  );
  
  // Output xml markup
  print "<title>" . check_plain($element['title']) . "</title>";
  print "<creator>" . check_plain($element['author']) . "</creator>";
  print "<location>" . check_url($element['fileurl']) . "</location>";
  print "<annotation>" . check_plain($element['description']) . "</annotation>";

  // Only add duration if it contains something (drupal.org/node/581470#comment-4147934 by David Stosik)
  if ($duration) {
    print "<duration>" . check_plain($duration) . "</duration>";
  }
  
  // If there is an image then use it
  if ($element['image']) {
    
    // If imagecache is enabled see if we need to use a modified thumbnail
    if ($imagecache) {
      $element['image'] = swftools_imagecache_create_path($imagecache, $element['image']);
    }

    // Output the resulting image
    print "<image>" . check_plain($element['image']) . "</image>";

  }
  
  // If this is a stream element then set meta
  if ($element['stream']) {
    print '<meta rel="streamer">' . check_plain($element['stream']) . '</meta>';
  }
    
?>

</track>
