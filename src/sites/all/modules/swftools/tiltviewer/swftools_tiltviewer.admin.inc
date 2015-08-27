<?php

/**
 * @file
 * Configuration settings for TiltViewer.
 */

function swftools_tiltviewer_admin_form() {

  // Fetch the form by using the profile definition
  $form = swftools_tiltviewer_profile_form();
  
  // Add custom form handler to flush cache upon submit
  $form['#submit'][] = 'swftools_admin_settings_submit';
  
  // Return finished form
  return system_settings_form($form);

}

function swftools_tiltviewer_profile_form($profile = '') {
  
  // See if colorpicker 2 is loaded
  $colorfield = function_exists('colorpicker_2_or_later') ? 'colorpicker_textfield' : 'textfield';
  
  // Retrieve current defaults
  $saved_settings = _swftools_tiltviewer_settings($profile);
  
  // Switch 0x for # in front of colors
  swftools_tiltviewer_admin_color_settings($saved_settings['basic'], SWFTOOLS_ADMIN_RETRIEVE);
  
  // Initialise tree from this point forward as want to store an array
  $form['swftools_tiltviewer']['#tree'] = TRUE;
  
  $form['swftools_tiltviewer']['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#description' => t('These settings control how the TiltViewer will look when it is displayed. Flickr integration sections are at the bottom of this form.'),
  );
  
  $form['swftools_tiltviewer']['basic']['maxJPGSize'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $saved_settings['basic']['maxJPGSize'],
    '#title' => t('Maximum jpeg size'),
    '#description' => t('Set this value to the largest dimension (width or height) of your largest image. TiltViewer uses this value to proportionately scale your images to fit.'),
  );
  
  $form['swftools_tiltviewer']['basic']['useReloadButton'] = array(
    '#type' => 'radios',
    '#title' => t('Reload button'),
    '#options' => array(
      'false' => t('Disabled'),
      'true' => t('Enabled'),
    ),
    '#description' => t('When enabled circular reload button will be shown under the image. When disabled next/previous buttons will be shown.'),
    '#default_value' => $saved_settings['basic']['useReloadButton'],
  );
  
  $form['swftools_tiltviewer']['basic']['showFlipButton'] = array(
    '#type' => 'radios',
    '#title' => t('Flip button'),
    '#options' => array(
      'false' => t('Disabled'),
      'true' => t('Enabled'),
    ),
    '#description' => t('When enabled the flip button will be overlayed and images can be flipped to see the text on the reverse side.'),
    '#default_value' => $saved_settings['basic']['showFlipButton'],
  );
  
  $form['swftools_tiltviewer']['basic']['showLinkButton'] = array(
    '#type' => 'radios',
    '#title' => t('Link button'),
    '#options' => array(
      'false' => t('Disabled'),
      'true' => t('Enabled'),
    ),
    '#description' => t('When enabled a \'go to Flickr\' button will be placed on the images\' flipside.'),
    '#default_value' => $saved_settings['basic']['showLinkButton'],
  );
  
  $form['swftools_tiltviewer']['basic']['frameColor'] = array(
    '#type' => $colorfield,
    '#default_value' => $saved_settings['basic']['frameColor'],
    '#size' => 8,
    '#maxlength' => 8,
    '#title' => t('Frame color'),
    '#description' => t('The hexadecimal color of image frame, in the form #FFFFFF.'),
  );
  
  $form['swftools_tiltviewer']['basic']['backColor'] = array(
    '#type' => $colorfield,
    '#default_value' => $saved_settings['basic']['backColor'],
    '#size' => 8,
    '#maxlength' => 8,
    '#title' => t('Flip side background color'),
    '#description' => t('The hexadecimal color of the flip side of the image, in the form #FFFFFF.'),
  );
  
  $form['swftools_tiltviewer']['basic']['bkgndInnerColor'] = array(
    '#type' => $colorfield,
    '#default_value' => $saved_settings['basic']['bkgndInnerColor'],
    '#size' => 8,
    '#maxlength' => 8,
    '#title' => t('Background inner color'),
    '#description' => t('The hexadecimal color of the stage background gradient center, in the form #FFFFFF.'),
  );
  
  $form['swftools_tiltviewer']['basic']['bkgndOuterColor'] = array(
    '#type' => $colorfield,
    '#default_value' => $saved_settings['basic']['bkgndOuterColor'],
    '#size' => 8,
    '#maxlength' => 8,
    '#title' => t('Background outer color'),
    '#description' => t('The hexadecimal color of the stage background gradient edge, in the form #FFFFFF.'),
  );
  
  $form['swftools_tiltviewer']['basic']['columns'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $saved_settings['basic']['columns'],
    '#title' => t('Columns'),
    '#description' => t('Number of columns to display.'),
  );

  $form['swftools_tiltviewer']['basic']['rows'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $saved_settings['basic']['rows'],
    '#title' => t('Rows'),
    '#description' => t('Number of rows to display.'),
  );
  
  $form['swftools_tiltviewer']['basic']['linkLabel'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['basic']['linkLabel'],
    '#title' => t('Link label'),
    '#description' => t('Enter the text to display as the flipside link button label. The default is %default.', array('%default' => 'go to Flickr')),
  );
  
  $form['swftools_tiltviewer']['basic']['langGoFull'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['basic']['langGoFull'],
    '#title' => t('Full screen label'),
    '#description' => t('Enter the text to display for the right click menu to enter full screen mode. The default is %default.', array('%default' => 'Go Fullscreen')),
  );
  
  $form['swftools_tiltviewer']['basic']['langExitFull'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['basic']['langExitFull'],
    '#title' => t('Exit full screen label'),
    '#description' => t('Enter the text to display for the right click menu to leave full screen mode. The default is %default.', array('%default' => 'Exit Fullscreen')),
  );
  
  $form['swftools_tiltviewer']['basic']['langAbout'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['basic']['langAbout'],
    '#title' => t('About label'),
    '#description' => t('Enter the text to display for the right click menu \'About\' menu. The default is %default.', array('%default' => 'About')),
  );

  // TODO: Add pro options
  
  $form['swftools_tiltviewer']['flickr'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flickr integration'),
    '#description' => t('When enabled this TiltViewer will default to fetching images from Flickr using the options below rather than generating a gallery using an image list.'),
  );
  
  $form['swftools_tiltviewer']['flickr']['useFlickr'] = array(
    '#type' => 'radios',
    '#title' => t('Default to Flickr integration'),
    '#options' => array(
      'false' => t('Disabled'),
      'true' => t('Enabled'),
    ),
    '#default_value' => $saved_settings['flickr']['useFlickr'],
  );
  
  $form['swftools_tiltviewer']['flickr']['user_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Flickr user id'),
    '#description' => t('Enter the user id for the Flickr gallery you want to view. Flickr user id\'s are of the form @userid. Leave this setting blank to retrieve from all available public images.', array('@userid' => '12345678@N00')),
    '#default_value' => $saved_settings['flickr']['user_id'],
  );
  
  $form['swftools_tiltviewer']['flickr']['tags'] = array(
    '#type' => 'textfield',
    '#title' => t('Tags'),
    '#description' => t('To limit the results returned from Flickr enter a comma separated list of tags, or leave blank to retrieve from all available public images'),
    '#default_value' => $saved_settings['flickr']['tags'],
  );
  
  $form['swftools_tiltviewer']['flickr']['tag_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Tag mode'),
    '#options' => array(
      'any' => t('Any'),
      'all' => t('All'),
    ),
    '#description' => t('If multiple tags are set then they can be combined so that photos containing any of the tags, or only photos containing all the tags, are retrieved.'),
    '#default_value' => $saved_settings['flickr']['tag_mode'],
  );
  
  $form['swftools_tiltviewer']['flickr']['showTakenByText'] = array(
    '#type' => 'radios',
    '#title' => t('Taken by text'),
    '#options' => array(
      'false' => t('Disabled'),
      'true' => t('Enabled'),
    ),
    '#description' => t('When enabled the flip side of the photo will show \'Taken by X on Y\'.'),
    '#default_value' => $saved_settings['flickr']['showTakenByText'],
  );
  
  // If imagecache is available show options, otherwise just store the current value
  if (module_exists('imagecache')) {

    $form['swftools_tiltviewer']['imagecache'] = array(
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
    
    $form['swftools_tiltviewer']['imagecache']['imagecache'] = array(
      '#type' => 'select',
      '#title' => t('Image cache preset'),
      '#default_value' => $saved_settings['imagecache']['imagecache'],
      '#options' => $options,
      '#description' => t('Select the imagecache preset that should be applied to images used in this player. If the images are being scaled by image cache then the <em>Maximum jpeg size</em> setting (above) should be changed to match this in order that the images display with the proper spacing.'),
    );

  }
  else {
    
    $form['swftools_tiltviewer']['imagecache']['imagecache'] = array(
      '#type' => 'value',
      '#value' => $saved_settings['imagecache']['imagecache'],
    );
    
  }
  
  // Add custom form handler to convert 1/0 to yes/no, and to store colors without leading #
  $form['#submit'][] = 'swftools_tiltviewer_admin_form_submit';
  
  // Return the finished form
  return $form;
  
}


/**
 * Adds, or removes, the leading # from a color setting.
 * 
 * The colorpicker modules like to prepend the color with a #, but
 * TiltViewer wants to receive colors without any leading character.
 * This function will add, or remove, the # from an array of color
 * settings.
 * 
 * @param array $settings
 *   An array of color settings.
 * @param int $mode
 *   Use one of the following constants:
 *   - SWFTOOLS_ADMIN_STORE: strip leading # and replace with 0x
 *   - SWFTOOLS_ADMIN_RETRIEVE: strip leading 0x and replace with #
 *   
 * @return nothing
 *   The array is passed by reference.
 */
function swftools_tiltviewer_admin_color_settings(&$settings, $mode) {

  // Array of color settings
  $colors = array(
    'frameColor',
    'backColor',
    'bkgndInnerColor',
    'bkgndOuterColor',
  );
  
  // Iterate and make the swap
  foreach ($colors as $color) {
    if ($settings[$color] != '') {
      if ($mode == SWFTOOLS_ADMIN_STORE) {
        $settings[$color] = str_replace('#', '0x', $settings[$color]);
      }
      else {
        $settings[$color] = str_replace('0x', '#', $settings[$color]);
      }
    }
  }
  
}


/**
 * Custom form handler to process color settings.
 */
function swftools_tiltviewer_admin_form_submit($form, &$form_state) {
  
  // Switch # for 0x in front of colors
  swftools_tiltviewer_admin_color_settings($form_state['values']['swftools_tiltviewer']['basic'], SWFTOOLS_ADMIN_STORE);
  
}
