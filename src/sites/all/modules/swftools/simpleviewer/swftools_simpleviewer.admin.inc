<?php

/**
 * @file
 * Configuration settings for SimpleViewer.
 */

function swftools_simpleviewer_admin_form() {

  // Fetch the form by using the profile definition
  $form = swftools_simpleviewer_profile_form();
  
  // Add custom form handler to flush cache upon submit
  $form['#submit'][] = 'swftools_admin_settings_submit';
  
  // Return finished form
  return system_settings_form($form);

}





function swftools_simpleviewer_profile_form($profile = '') {
  
  // See if colorpicker 2 is loaded
  $colorfield = function_exists('colorpicker_2_or_later') ? 'colorpicker_textfield' : 'textfield';
  
  // Retrieve current defaults
  $saved_settings = _swftools_simpleviewer_settings($profile);

  // Initialise tree from this point forward as want to store an array
  $form['swftools_simpleviewer']['#tree'] = TRUE;
  
  $form['swftools_simpleviewer']['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
    '#description' => t('These settings control how the SimpleViewer will look when it is displayed. Heights and widths should be given in pixels.'),
  );
  
  $form['swftools_simpleviewer']['appearance']['galleryStyle'] = array(
    '#type' => 'radios',
    '#options' => array(
      'MODERN' => t('Modern'),
      'COMPACT' => t('Compact'),
      'CLASSIC' => t('Classic'),
    ),
    '#default_value' => $saved_settings['appearance']['galleryStyle'],
    '#title' => t('Gallery style'),
    '#description' => t('Choose the gallery style that this SimpleViewer should use.'),  
  );
  
  $form['swftools_simpleviewer']['appearance']['width'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['appearance']['width'],
    '#size' => 5,
    '#maxlength' => 5,
    '#title' => t('Width of SimpleViewer'),
    '#required' => TRUE,
  );
  
  $form['swftools_simpleviewer']['appearance']['height'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['appearance']['height'],
    '#size' => 5,
    '#maxlength' => 5,
    '#title' => t('Height of SimpleViewer'),
    '#required' => TRUE,
  );
  
  $form['swftools_simpleviewer']['appearance']['maxImageWidth'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['appearance']['maxImageWidth'],
    '#size' => 5,
    '#maxlength' => 5,
    '#title' => t('Width of your largest image in pixels'),
    '#required' => TRUE,
    '#description' => t('Used to determine the best layout for your gallery.'),
  );
  
  $form['swftools_simpleviewer']['appearance']['maxImageHeight'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['appearance']['maxImageHeight'],
    '#size' => 5,
    '#maxlength' => 5,
    '#title' => t('Height of your largest image in pixels'),
    '#required' => TRUE,
    '#description' => t('Used to determine the best layout for your gallery.'),
  );
  
  $form['swftools_simpleviewer']['appearance']['frameWidth'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['appearance']['frameWidth'],
    '#size' => 5,
    '#maxlength' => 5,
    '#title' => t('Width of image frame in pixels'),
    '#required' => TRUE,
  );
  
  $form['swftools_simpleviewer']['appearance']['thumbPosition'] = array(
    '#type' => 'radios',
    '#options' => array(
      'TOP' => t('Top'),
      'BOTTOM' => t('Bottom'),
      'RIGHT' => t('Right'),
      'LEFT' => t('Left'),
      'NONE' => t('None'),
    ),
    '#default_value' => $saved_settings['appearance']['thumbPosition'],
    '#title' => t('Thumbnail position'),
    '#description' => t('Position of thumbnails relative to image.'),
  );
  
  $form['swftools_simpleviewer']['appearance']['thumbColumns'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $saved_settings['appearance']['thumbColumns'],
    '#title' => t('Thumbnail columns'),
    '#description' => t('Number of thumbnail columns. To disable thumbnails completely set this value to 0'),
  );

  $form['swftools_simpleviewer']['appearance']['thumbRows'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $saved_settings['appearance']['thumbRows'],
    '#title' => t('Thumbnail rows'),
    '#description' => t('Number of thumbnail rows. To disable thumbnails completely set this value to 0.'),
  );
  
  $form['swftools_simpleviewer']['appearance']['showOpenButton'] = array(
    '#type' => 'radios',
    '#options' => array(
      'false' => t('Disabled'),
      'true' => t('Enabled'),
    ),
    '#default_value' => $saved_settings['appearance']['showOpenButton'],
    '#title' => t('Open button'),
    '#description' => t('When enabled a button and context menu to allow the image to be opened in a new window will be included.'),
  );
  
    $form['swftools_simpleviewer']['appearance']['showFullScreenButton'] = array(
    '#type' => 'radios',
    '#options' => array(
      'false' => t('Disabled'),
      'true' => t('Enabled'),
    ),
    '#default_value' => $saved_settings['appearance']['showFullScreenButton'],
    '#title' => t('Full screen button'),
    '#description' => t('When enabled a button and context menu to allow full screen mode to be used will be included.'),
  );
  
  $form['swftools_simpleviewer']['appearance']['textColor'] = array(
    '#type' => $colorfield,
    '#default_value' => $saved_settings['appearance']['textColor'],
    '#size' => 8,
    '#maxlength' => 7,
    '#title' => t('Text color'),
    '#description' => t('Color of title and caption text. This should be a hexadecimal number in the form #FFFFFF.'),
    '#required' => TRUE,
  );
  
  $form['swftools_simpleviewer']['appearance']['frameColor'] = array(
    '#type' => $colorfield,
    '#default_value' => $saved_settings['appearance']['frameColor'],
    '#size' => 8,
    '#maxlength' => 7,
    '#title' => t('Frame color'),
    '#description' => t('Color of image frame, navigation buttons (on top and hover buttons) and thumbnail frame. This should be a hexadecimal number in the form #FFFFFF.'),
    '#required' => TRUE,
  );
  
  $form['swftools_simpleviewer']['flickr'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flickr integration'),
    '#description' => t('When enabled this SimpleViewer will default to fetching images from Flickr using the options below rather than generating a gallery using an image list.'),
  );
  
  $form['swftools_simpleviewer']['flickr']['useFlickr'] = array(
    '#type' => 'radios',
    '#title' => t('Default to Flickr integration'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => $saved_settings['flickr']['useFlickr'],
  );
  
  $form['swftools_simpleviewer']['flickr']['flickrUserName'] = array(
    '#type' => 'textfield',
    '#title' => t('Flickr username'),
    '#description' => t('Enter the username for the Flickr gallery you want to view, or leave blank to retrieve from all available public images.'),
    '#default_value' => $saved_settings['flickr']['flickrUserName'],
  );
  
  $form['swftools_simpleviewer']['flickr']['flickrTags'] = array(
    '#type' => 'textfield',
    '#title' => t('Tags'),
    '#description' => t('To limit the results returned from Flickr enter a comma separated list of tags, or leave blank to retrieve from all available public images.'),
    '#default_value' => $saved_settings['flickr']['flickrTags'],
  );
  
  // If imagecache is available show options, otherwise just store the current value
  if (module_exists('imagecache')) {
    
    $form['swftools_simpleviewer']['imagecache'] = array(
      '#type' => 'fieldset',
      '#title' => t('Image cache'),
      '#description' => t('Image cache can be used to resize uploaded images before they are displayed in the viewer. This can help to reduce bandwith and speed up page loading.'),
    );
    
    $options = array(
      SWFTOOLS_UNDEFINED => t('None'),
    );
    
    foreach (imagecache_presets() as $preset) {
      $options[$preset['presetname']] = $preset['presetname'];
    };
    
    $form['swftools_simpleviewer']['imagecache']['imagecache'] = array(
      '#type' => 'select',
      '#title' => t('Image cache preset'),
      '#default_value' => $saved_settings['imagecache']['imagecache'],
      '#options' => $options,
      '#description' => t('Select the imagecache preset that should be applied to images used in this player. If the images are being scaled by image cache then the <em>width and height of your largest image</em> settings (above) should be changed to match this in order that the images display with the proper spacing.'),
    );
    
  }
  else {
    $form['swftools_simpleviewer']['imagecache']['imagecache'] = array(
      '#type' => 'value',
      '#value' => $saved_settings['imagecache']['imagecache'],
    );
  }
  
  return $form;
  
}





