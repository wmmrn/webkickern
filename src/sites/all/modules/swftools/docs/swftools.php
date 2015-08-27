<?php

/**
 * @file
 * Doxygen documentation for SWF Tools.
 */

/**
 * @defgroup swftools SWF Tools
 * @{
 * Enables comprehensive support for Flash based media.
 *
 * SWF Tools provides a convenient way to enable Flash based media within a
 * Drupal website. It can be used to place a simple .swf file on the page, and the
 * module is easily extended with other components to enhance its capabilities.
 * Modules are available to enable play back of audio, video and image based
 * content through a variety of Flash based media players.
 * 
 * The features of SWF Tools can be accessed in a number of ways.
 * 
 * For simple display of a single file directly within a node the module makes
 * an SWF Tools input filter available. Enable this for an input format and
 * Flash, audio and video can be rendered directly from within the node using
 * [swf file="myFile.swf"] or [swf files="myAudio.mp3&&moreAudio.mp3&&otherAudio.mp3"]
 *  
 * From within PHP SWF Tools is accessed via a simple function call such as 
 *  
 * @code $output = swf($file, $options); @endcode
 * 
 * It is also possible to include Flash, audio or video within a form definition
 * by using the swftools element.
 * 
 * @code $form['mySWF'] = array(
 *   '#type' => 'swftools',
 *   '#value' => 'myFlash.swf',
 * ); @endcode
 * 
 * SWF Tools has built in integration with CCK and Views. By default it will
 * expose some basic formatters that can be used with file fields, text fields
 * and link fields. These allow the fields to be rendered in to a series of discrete
 * players, or assembled in to a playlist.
 * 
 * With the addition of the SWF Tools Profiles module different player setups
 * can be created and these are all exposed as additional formatters. This makes
 * it possible to have different styles of player associated with different CCK
 * content types.
 * 
 * Integration with Views lets SWF Tools act as a handler to render a query into
 * a playlist. For example, a query could extract all the uploaded files from a
 * content type and return the five most recent items. The results can be
 * passed to any player profile to be rendered. This makes it very simple to
 * create a page or a block that shows customised views.
 * 
 * With players like LongTail Player and FlowPlayer 3 playlists can include a
 * thumbnail image. This will be displayed in the player when it is waiting for
 * the content to play, or as a background image when audio is playing.
 * 
 * SWF Tools supports a number of image gallery modules, such as SimpleViewer
 * and LongTail Image Rotator. These can be used directly in a node as a
 * formatter for a CCK content type, but they are most effective when used in
 * conjunction with Views to generate a Flash based slideshow.
 * 
 * User topics:
 * 
 * Developer topics:
 * - @link hook_swftools_methods Registering players with SWF Tools @endlink
 */

/**
 * Reports an embedding method or a player to SWF Tools.
 * 
 * Embedding methods and players should implement hook_swftools_methods() to
 * register themselves with SWF Tools.
 * 
 */
function hook_swftools_methods() {
  
  /**
   * First define the player as an array of properties.
   */
  $myplayer = array(
    'module'      => 'swftools_myplayer',
    'version'     => 9,
    'title'       => t('My Player'),
    'download'    => 'http://myplayer.org',
    'width'       => 500,
    'height'      => 375,
    'library'     => swftools_get_library('myplayer') . 'myplayer/myplayer.swf',
    'profile'     => array(
      'path' => 'myplayer',
      'settings' => array('swftools_myplayer', 'swftools_myplayer_colors'),
      'file' => 'swftools_myplayer.admin.inc',
      'page argument' => 'swftools_myplayer_form',
    ),
  );
  
  /**
   * Then associate the player with actions that it can process.
   * [action][name_of_player] = [player_details]
   */
  $methods['video']['myplayer'] = $myplayer;
  $methods['video_list']['myplayer'] = $myplayer;
  $methods['audio']['myplayer'] = $myplayer;
  $methods['audio_list']['myplayer'] = $myplayer;
  $methods['image_list']['myplayer'] = $myplayer;
  $methods['swftools_media_display']['myplayer'] = $myplayer;
  $methods['media_list']['myplayer'] = $myplayer;

  // Return the method results
  return $methods;

}


/**
 * Processes a playlist ready for use by the specified player.
 * 
 * If a player can process a playlist then it should implement this
 * hook to perform processing that turns the SWF Tools array of data in to the
 * format that the player will ultimately use.
 * 
 * If the player uses xml based playlists then this hook should return a string
 * of markup. SWF Tools will take care of storing the playlist and giving it a
 * file name. The filename will be available on the ['othervars']['file_url'] key
 * when the player is ready to be rendered.
 * The filename is a 32 character hexadecimal string that is built by hashing the
 * playlist with the specified player options. This should generate a unique
 * string for a given playlist in a given player. Playlists are not physically
 * written to the file system but are stored in the table {cache_swftools}. All
 * the player needs to do is assign the ['othervars']['url'] value to access it.
 * What actually happens is that SWF Tools is providing a menu path. When the
 * browser requests the file SWF Tools serves it directly from the cache. When the
 * site caches are flushed SWF Tools will rebuild the playlists when they are next
 * requested.
 * 
 * Players such as FlowPlayer 3 do not use an xml based playlist system. Instead
 * they use a configuration array that is written out directly as part of the player
 * configuration. In these cases the processing does not necessarily have to be
 * done in hook_swftools_playlist_PLAYER() and it could be handled in
 * hook_swftools_preprocess_PLAYER(). However, it is a good idea to use the
 * playlist hook as it separates the logic of preparing a playlist from the logic of
 * outputting the player. When preparing a non-xml playlist the code in this hook
 * should directly modify $data, and it should return the value SWFTOOLS_NON_XML_PLAYLIST
 * to notify SWF Tools that xml generation should be skipped.
 * 
 * For examples of xml based playlist generation see:
 * - swftools_wijering4_swftools_playlist_jwplayer4()
 * - swftools_simpleviewer_swftools_playlist_simpleviewer()
 * 
 * For examples of non-xml based playlist generation see:
 * - swftools_wpaudio_swftools_playlist_wpaudio()
 * - swftools_flowplayer3_swftools_playlist_flowplayer3()
 * 
 * @param array $data
 *   An array of SWF Tools data representing the media that is to be rendered.
 *   
 * @see hook_swftools_preprocess_PLAYER()
 */
function hook_swftools_playlist_PLAYER(&$data) {

  // Get array of image rotator settings
  $saved_settings = _swftools_PLAYER_settings($data['othervars']['profile']);
  
  // Initialise a string to contain the elements
  $xml = '';
  
  // Iterate over the playlist to build elements xml
  foreach ($data['othervars']['playlist_data']['playlist'] AS $track => $details) {
    
    // If imagecache is enabled see if we need to use a modified thumbnail
    if ($saved_settings['imagecache'] != SWFTOOLS_UNDEFINED) {
      $details['fileurl'] = swftools_imagecache_create_path($saved_settings['imagecache'], $details['fileurl']);
    }
    
    // Create an individual xml element on the raw playlist
    $xml .= theme('swftools_PLAYER_playlist_element', $details);
    
  }
  
  // Add xml wrapper around the elements
  $xml = theme('swftools_PLAYER_playlist_wrapper', $data['othervars']['playlist_data']['header'], $xml);
  
  // Return the resulting xml
  return $xml;

}


/**
 * Does the final preparation prior to rendering the specified player.
 * 
 * This hook is called just before the player is output on to the page. At this
 * point the SWF Tools data array is complete and the element is almost ready for
 * display.  Prior to this hook being called SWF Tools has been working out what
 * action is needed, and what player and embedding method is going to be used. It
 * is up to the implementing module to do the final preparations and configuration
 * of the Flash element. For example, it the player requires a specific flashvar to
 * be set in order to find the playlist then this should be done in this hook.
 * 
 * A common flow of logic in this hook is to determine whether a splash image is
 * being used, and if so assign it to a flashvar, followed by assignment of the
 * generated xml playlist to another flashvar.
 * 
 * More complex code is possible, and sometimes necessary. For example, the
 * FlowPlayer 3 module uses JSON notation to create a single flashvar that holds
 * the entire configuration. In that case the hook function is more complex since
 * it must assemble the JSON array from the constituent data in the $data array.
 * 
 * This function does not return anything - $data should be passed by reference and
 * then modified directly.
 * 
 * If any supporting JavaScript is required then it can be added via this hook.
 * Some care is needed when using JavaScript dependent players to make sure that the
 * JavaScript is always available on the finished page. In particular, if the site
 * allows content to be added via the input filter then this hook is only called the
 * first time the content is generated. The resulting markup is cached by the input
 * filter and on subsequent views the cached version will be used. The module must
 * therefore make sure it adds JavaScript to every page by using hook_init().
 * 
 * For examples of hook_swftools_preprocess_PLAYER() see:
 * - swftools_wijering4_swftools_preprocess_jwplayer4()
 * - swftools_flowplayer3_swftools_preprocess_flowplayer3()
 * 
 * @see hook_swftools_playlist_PLAYER()
 */
function hook_swftools_preprocess_PLAYER(&$data) {
  
  // Get current defaults for the player
  $saved_settings = _swftools_PLAYER_flashvars($data['othervars']['profile']);
  
  // Prepare an array of flashvars by merging defaults and user values
  $data['flashvars'] = array_merge($saved_settings, $data['flashvars']);
  
  // If an image has been set then use it  
  if ($data['othervars']['image']) {
   
    // Get source path to the image file
    $source = swftools_get_url_and_path($data['othervars']['image']);
    
    // If $source succeeded add image to the playlist
    if ($source) {
      
      // See if we need to apply an imagecache preset
      if ($saved_settings['imagecache_player'] != SWFTOOLS_UNDEFINED) {
        $source['fileurl'] = swftools_imagecache_create_path($saved_settings['imagecache_player'], $source['fileurl']);
      };

      // Store result in a flashvar called image
      $data['flashvars']['image'] = $source['fileurl'];

    }

  }
  
  // Don't output imagecache_variables
  unset($data['flashvars']['imagecache_player']);
  
  // Attach the generated path the xml based playlist
  $data['flashvars']['file'] = $data['othervars']['file_url'];
  
}



/**
 * @} End of "defgroup swftools".
 */
