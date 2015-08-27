<?php

/**
 * @file
 * Configuration settings for Flowplayer 3.
 */


/**
 * @addtogroup swftools
 * @{
 */

/**
 * Return the current palette for Flowplayer 3.
 */
define('SWFTOOLS_FLOWPLAYER3_CURRENT_PALETTE', 0x0000);

/**
 * Return the default palette for Flowplayer 3.
 */
define('SWFTOOLS_FLOWPLAYER3_DEFAULT_PALETTE', 0x0001);

/**
 * @} End of "addtogroup swftools"
 */

/**
 * Menu callback for the Flowplayer 3 settings form.
 */
function swftools_flowplayer3_admin_settings() {

  // Retrieve options needed to populate select elements
  $options = _swftools_flowplayer3_options();

  // Fetch form definition from the profile as our start point
  $form = swftools_flowplayer3_profile_form();

  // Player path
  $form['swftools_flowplayer3_files'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Player and plugin files'),
  );

  $form['swftools_flowplayer3_files']['swftools_flowplayer3_file'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('swftools_flowplayer3_file', SWFTOOLS_FLOWPLAYER3_FILE),
    '#title' => t('Player file'),
    '#description' => t('Specify the name of the player file - with Flowplayer 3 this typically changes with each new release.'),
    '#size' => 50,
    '#autocomplete_path' => 'admin/settings/swftools/flowplayer3/autocomplete',
  );

  $form['swftools_flowplayer3_files']['swftools_flowplayer3_stream_plugin'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('swftools_flowplayer3_stream_plugin', SWFTOOLS_FLOWPLAYER3_STREAM_PLUGIN),
    '#title' => t('RTMP streaming plugin'),
    '#description' => t('Specify the name of the rtmp streaming plugin.'),
    '#size' => 50,
    '#autocomplete_path' => 'admin/settings/swftools/flowplayer3/autocomplete',
  );

  $form['swftools_flowplayer3_files']['swftools_flowplayer3_javascript'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('swftools_flowplayer3_javascript', SWFTOOLS_FLOWPLAYER3_JAVASCRIPT),
    '#title' => t('Embedding script'),
    '#description' => t('The path to the Flowplayer 3 embedding script, if Flowplayer 3 embedding is enabled. When downloaded from flowplayer.org the script file is in a subdirectory <em>example</em> so a relative path may be needed when setting this value.'),
    '#size' => 50,
    '#autocomplete_path' => 'admin/settings/swftools/flowplayer3/autocomplete',
  );

  $form['swftools_flowplayer3_files']['swftools_flowplayer3_scrollable_javascript'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('swftools_flowplayer3_scrollable_javascript', SWFTOOLS_FLOWPLAYER3_SCROLLABLE_JAVASCRIPT),
    '#title' => t('Scrollable script'),
    '#description' => t('The path to the Flowplayer 3 tools.scrollable script.'),
    '#size' => 50,
    '#autocomplete_path' => 'admin/settings/swftools/flowplayer3/autocomplete',
  );
  
  $form['swftools_flowplayer3_files']['swftools_flowplayer3_controls'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('swftools_flowplayer3_controls', ''),
    '#title' => t('Controls'),
    '#description' => t('The path to the Flowplayer 3 controls plug-in. Leave blank to use the default.'),
    '#size' => 50,
    '#autocomplete_path' => 'admin/settings/swftools/flowplayer3/autocomplete',
  );

  $form['swftools_flowplayer3_files']['swftools_flowplayer3_product_key'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('swftools_flowplayer3_product_key', ''),
    '#title' => t('Product key'),
    '#description' => t('Enter your product key if you are using the commercial version of Flowplayer 3.'),
    '#size' => 50,
  );
  
  // Embedding setting
  $form['swftools_flowplayer3_embedding'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Flowplayer 3 embedding'),
  );

  $form['swftools_flowplayer3_embedding']['swftools_flowplayer3_embed'] = array(
    '#type' => 'radios',
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('swftools_flowplayer3_embed', 0),
    '#title' => t('Flowplayer 3 embedding'),
    '#description' => t('When enabled the default embedding method wil be over-ridden and the Flowplayer 3 embedding script will be used. The script file must be installed and the correct path added in <em>Player and plugin files</em> above.'),
  );

  $form['swftools_flowplayer3_embedding']['swftools_flowplayer3_load'] = array(
    '#type' => 'radios',
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('swftools_flowplayer3_load', 1),
    '#title' => t('Load Flowplayers immediately'),
    '#description' => t('If this setting is disabled then Flowplayers will not load until the container content is clicked. This feature requires Flowplayer 3 embedding to be enabled.'),
  );
  
  $form['swftools_flowplayer3_embedding']['swftools_flowplayer3_scrollable_script'] = array(
    '#type' => 'radios',
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => variable_get('swftools_flowplayer3_scrollable_script', 0),
    '#title' => t('tools.scrollable.js'),
    '#description' => t('This must be enabled in order to allow scrollable playlists to be used. This feature requires the <a href="http://drupal.org/project/jquery_update">jQuery update</a> module to be installed.', array('!url' => url('admin/settings/swftools/flowplayer3'))),
    '#disabled' => !swftools_flowplayer3_jquery_ok(),
    
  );
  
  // Add custom form handler to flush cache upon submit
  $form['#submit'][] = 'swftools_admin_settings_submit';

  // Return finished form
  return system_settings_form($form);

}


/**
 * Options used to populate the select elements of the form above
 * 
 * They are collected together here in a single array to make them
 * easier to find!
 * 
 * @return array
 *   An array of options for use on the flowplayer3 admin form.
 */
function _swftools_flowplayer3_options() {

  // Options for radios
  $options = array(

  'bool' => array(
      'true' => t('true'),
      'false' => t('false'),
    ),

    'enabled' => array(
      'true' => t('Enabled'),
      'false' => t('Disabled'),
    ),

    'accessible' => array(
      SWFTOOLS_ACCESSIBLE_DISABLED => t('Disabled'),
      SWFTOOLS_ACCESSIBLE_HIDDEN => t('Enabled and hidden'),
      SWFTOOLS_ACCESSIBLE_VISIBLE => t('Enabled and visible'),
    ),

    'visible' => array(
      1 => t('Visible'),
      0 => t('Hidden'),
    ),

    'scaling' => array(
      'fit' => t('Fit - fit to window by preserving the aspect ratio encoded in the file\'s metadata.'),
      'half' => t('Half - half-size (preserves aspect ratio)'),
      'orig' => t('Original - use the dimensions encoded in the file. If it is too big for the available space it is scaled using the \'fit\' option.'),
      'scale' => t('Scale - Scale the video to fill all available space. Ignores the dimensions in the metadata.'),
    ),

    'linkWindow' => array(
      '_blank' => t('_blank - a new window.'),
      '_parent' => t('_parent - the parent of the current window.'),
      '_self' => t('_self - the current frame in the current window.'),
      '_top' => t('_top - the top level frame in the current window.'), 
    ),

    'gradient' => array(
      'none' => t('None'),
      'low' => t('Low'),
      'medium' => t('Medium'),
      'high' => t('High'),
    ),

    'autoHide' => array(
      'never' => t('Never - the control bar is always shown.'),
      'always' => t('Always - the control bar will always hide itself when not in use.'),
      'fullscreen' => t('Fullscreen - the control will be only autohide during full screen playback.'), 
    ),

  'backgroundRepeat' => array(
      'no-repeat' => 'no-repeat',
      'repeat' => 'repeat',
    ),
  );
  
  return $options;

}


/**
 * Retrieve the color scheme information for Flowplayer 3 styling.
 *
 * @return
 *   An array of data defining color information and names for each preset style.
 */
function swftools_flowplayer3_get_info() {

  // Cache $info
  static $info;

  // If $info is populated, return it
  if (isset($info)) {
    return $info;
  }

  // Gather $info
  $path = drupal_get_path('module', 'swftools_flowplayer3');
  $file = $path . '/swftools_flowplayer3.colorschemes.inc';
  include $file;
  return $info;
}


/**
 * Helper function to retrieve the color palette for a particular style.
 *
 * @param int $default
 *   (optional) Use one of the following constants:
 *   - SWFTOOLS_FLOWPLAYER3_DEFAULT_PALETTE
 *   - SWFTOOLS_FLOWPLAYER_CURRENT_PALETTE
 *   
 * @return
 *   An array of key/value pairs - the key defines the componenent and the
 *   value defines the color.
 */
function swftools_flowplayer3_get_palette($default = SWFTOOLS_FLOWPLAYER3_CURRENT_PALETTE, $profile = '') {

  // Define palette properties
  $fields = array(
    'backgroundColor',
    'controlbarbackgroundColor',
    'timeColor',
    'durationColor',
    'progressColor',
    'bufferColor',
    'sliderColor',
    'buttonColor',
    'buttonOverColor',
    'volumeSliderColor',
    'timeBgColor',
  );

  // Get the theme information
  $info = swftools_flowplayer3_get_info();

  // Get the keys (strings of colors) from the available schemes
  $keys = array_keys($info['schemes']);

  // Exploded the first (default) string in an array
  foreach (explode(',', array_shift($keys)) as $k => $scheme) {
    // Build a palette
    $palette[$fields[$k]] = $scheme;
  }

  // If default is required, return it, otherwise load palette variable
  return $default ? $palette : swftools_variable_get('swftools_flowplayer3_palette', $palette, $profile);
}


/**
 * Helper function to produce the color scheme form for the flowplayer
 */
function swftools_flowplayer3_scheme_form(&$form_state, $profile = '') {

  // Get path to the flowplayer3 module
  $base = drupal_get_path('module', 'swftools_flowplayer3');

  // Add Farbtastic color picker
  drupal_add_css('misc/farbtastic/farbtastic.css');
  drupal_add_js('misc/farbtastic/farbtastic.js');

  // Add custom CSS/JS
  drupal_add_css($base . '/swftools_flowplayer3.colorpicker.css');
  drupal_add_js($base . '/swftools_flowplayer3.colorpicker.js');
  drupal_add_js(array('color' => array(
    'reference' => swftools_flowplayer3_get_palette(SWFTOOLS_FLOWPLAYER3_DEFAULT_PALETTE, $profile)
  )), 'setting');

  // Get info about the current theme color scheme
  $info = swftools_flowplayer3_get_info();

  // See if we're using a predefined scheme
  // The variable is empty if using the default, or an array of key/value pairs otherwise
  $current = implode(',', swftools_variable_get('swftools_flowplayer3_palette', array(), $profile));

  // Note: we use the original theme when the default scheme is chosen.
  $current = isset($info['schemes'][$current]) ? $current : ($current == '' ? reset($info['schemes']) : '');

  // Add custom to scheme selector options
  $info['schemes'][''] = t('Custom');

  // Add scheme selector to form (the value is a comma separated string of hex colors)
  $form['swftools_flowplayer3_scheme'] = array(
    '#type' => 'select',
    '#title' => t('Color set'),
    '#options' => $info['schemes'],
    '#default_value' => $current,
  );

  // Get current palette as an array
  $palette = swftools_flowplayer3_get_palette(SWFTOOLS_FLOWPLAYER3_CURRENT_PALETTE, $profile);

  // Build an array of titles to be used for each palette element
  $names = array(
    'backgroundColor' => array(
      '#title' => t('Background color'),
  ),
    'controlbarbackgroundColor' => array(
      '#title' => t('Control bar background color'),
  ),
    'timeColor' => array(
      '#title' => t('Elapsed time font color'),
  ),
    'durationColor' => array(
      '#title' => t('Total time font color'),
  ),
    'progressColor' => array(
      '#title' => t('Progress bar color'),
  ),
    'bufferColor' => array(
      '#title' => t('Buffer color'),
  ),
    'sliderColor' => array(
      '#title' => t('Slider color'),
  ),
    'buttonColor' => array(
      '#title' => t('Button color'),
  ),
    'buttonOverColor' => array(
      '#title' => t('Button over color'),
  ),
    'volumeSliderColor' => array(
      '#title' => t('Volume slider color'),
  ),
    'timeBgColor' => array(
      '#title' => t('Time background color'),
  ),
  );

  // Form elements to be part of a tree
  $form['swftools_flowplayer3_palette']['#tree'] = true;

  // Cycle through each palette element
  foreach ($palette as $name => $value) {
    $form['swftools_flowplayer3_palette'][$name] = array(
      '#type' => 'textfield',
      '#title' => $names[$name]['#title'],
      '#default_value' => $value,
      '#size' => 8,
    );
  }

  // Return the form
  return $form;
}


/**
 * Theme the Flowplayer 3 color settings for the admin form.
 */
function theme_swftools_flowplayer3_scheme_form($form, $profile = '') {

  $output = '';

  // Wrapper
  $output .= '<div class="color-form clear-block">';

  // Color schemes
  $output .= drupal_render($form['swftools_flowplayer3_scheme']);

  // Palette
  $output .= '<div id="palette" class="clear-block">';
  foreach (element_children($form['swftools_flowplayer3_palette']) as $name) {
    $output .= drupal_render($form['swftools_flowplayer3_palette'][$name]);
  }
  $output .= '</div>';

  // Close wrapper
  $output .= '</div>';

  return $output;
}


function swftools_flowplayer3_profile_form($profile = '') {

  // Retrieve combination of default and saved settings
  $saved_settings = _swftools_flowplayer3_settings($profile);

  // Retrieve options needed to populate select elements
  $options = _swftools_flowplayer3_options();

  // Initialise array to hold form
  $form = array();

  // Add color scheme form
  $form['color'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Color scheme'),
    '#weight' => -1,
    '#attributes' => array('id' => 'swftools_flowplayer3_scheme_form'),
    '#theme' => 'swftools_flowplayer3_scheme_form',
  );

  $form['color'] += swftools_flowplayer3_scheme_form($form_state, $profile);

  // Initialise tree from this point forward as want to store arrays
  $form['swftools_flowplayer3']['#tree'] = TRUE;

  // Clip settings
  $form['swftools_flowplayer3']['clip'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Overall clip properties'),
  );

  $form['swftools_flowplayer3']['clip']['autoPlay'] = array(
    '#type' => 'radios',
    '#options' => $options['enabled'],
    '#default_value' => $saved_settings['clip']['autoPlay'],
    '#title' => t('Auto play'),
    '#description' => t('When enabled the player will being playback immediately after it has finished loading.'),
  );

  $form['swftools_flowplayer3']['clip']['autoBuffering'] = array(
    '#type' => 'radios',
    '#options' => $options['enabled'],
    '#default_value' => $saved_settings['clip']['autoBuffering'],
    '#title' => t('Auto buffering'),
    '#description' => t('When enabled loading of clip into the player\'s memory will begin as soon as the player is loaded on the page.'),
  );

  $form['swftools_flowplayer3']['clip']['scaling'] = array(
    '#type' => 'radios',
    '#options' => $options['scaling'],
    '#default_value' => $saved_settings['clip']['scaling'],
    '#title' => t('Scaling'),
    '#description' => t('This determines how a video clip should be scaled if it is bigger than the player canvas.'),
  );

  $form['swftools_flowplayer3']['clip']['start'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['clip']['start'],
    '#title' => t('Start'),
    '#description' => t('Specify start point for playback (only works with a streaming server). Streaming support not yet in place for SWF Tools.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['clip']['duration'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['clip']['duration'],
    '#title' => t('Duration'),
    '#description' => t('The time (in seconds) for which a video clip should be played until playback ceases.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['clip']['accelerated'] = array(
    '#type' => 'radios',
    '#options' => $options['enabled'],
    '#default_value' => $saved_settings['clip']['accelerated'],
    '#title' => t('Acceleration'),
    '#description' => t('When enabled browsers using Flash 9 or higher will use hardware-accelerated full screen mode.'),
  );

  $form['swftools_flowplayer3']['clip']['bufferLength'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['clip']['bufferLength'],
    '#title' => t('Buffer length'),
    '#description' => t('The amount of video data (in seconds) which should be loaded into Flowplayer\'s memory in advance of playback commencing. '),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['clip']['provider'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['clip']['provider'],
    '#title' => t('Provider'),
    '#description' => t('Not sure if this will be supported in this module. Need to investigate.'),
  );

  $form['swftools_flowplayer3']['clip']['fadeInSpeed'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['clip']['fadeInSpeed'],
    '#title' => t('Fade in speed'),
    '#description' => t('Time in milliseconds to fade from zero to full opacity upon commencement of video playback.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['clip']['fadeOutSpeed'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['clip']['fadeOutSpeed'],
    '#title' => t('Fade out speed'),
    '#description' => t('Time in milliseconds to fade from full to zero opacity upon completion of video playback.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['clip']['linkUrl'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['clip']['linkUrl'],
    '#title' => t('Link url'),
    '#description' => t('The URL to direct users to when clicking the video screen.'),
  );

  $form['swftools_flowplayer3']['clip']['linkWindow'] = array(
    '#type' => 'radios',
    '#options' => $options['linkWindow'],
    '#default_value' => $saved_settings['clip']['linkWindow'],
    '#title' => t('Link window'),
    '#description' => t('This determines where the url defined by the linkUrl setting will open.'),
  );

  $form['swftools_flowplayer3']['clip']['live'] = array(
    '#type' => 'radios',
    '#options' => $options['enabled'],
    '#default_value' => $saved_settings['clip']['live'],
    '#title' => t('Live'),
    '#description' => t('RTMP streaming servers support live streaming, which means that it is possible to setup a video camera or other live video source to stream live video data to Flowplayer. If you have an RTMP server which is providing a live video stream, you should set this property to enabled. Streaming support not yet in place for SWF Tools.'),
  );

  $form['swftools_flowplayer3']['clip']['cuePointMultiplier'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['clip']['cuePointMultiplier'],
    '#title' => t('Cue point multiplier'),
    '#description' => t('The times of embedded cuepoints are multiplied by this value before being used. Some encoding tools embed cuepoints in seconds but Flowplayer processes cuepoints in milliseconds. Note that the times are also rounded to the nearest 100 milliseconds. For example, if a file has a cuepoint at 5.535 seconds, the cuepoint is fired at 5500 milliseconds (assuming the default multiplier value of 1000).'),
    '#size' => 8,
  );

  // Control bar settings
  $form['swftools_flowplayer3']['controls'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Controlbar properties'),
  );

  $form['swftools_flowplayer3']['controls']['backgroundGradient'] = array(
    '#type' => 'select',
    '#options' => $options['gradient'],
    '#default_value' => $saved_settings['controls']['backgroundGradient'],
    '#title' => t('Background gradient'),
    '#description' => t('The control bar can be displayed with a fade that runs vertically, or if set to none it will be a flat color.'),
  );

  $form['swftools_flowplayer3']['controls']['progressGradient'] = array(
    '#type' => 'select',
    '#options' => $options['gradient'],
    '#default_value' => $saved_settings['controls']['progressGradient'],
    '#title' => t('Progress gradient'),
    '#description' => t('The progress bar can be displayed with a fade that runs vertically, or if set to none it will be a flat color.'),
  );

  $form['swftools_flowplayer3']['controls']['bufferGradient'] = array(
    '#type' => 'select',
    '#options' => $options['gradient'],
    '#default_value' => $saved_settings['controls']['bufferGradient'],
    '#title' => t('Buffer gradient'),
    '#description' => t('The buffer progress can be displayed with a fade that runs vertically, or if set to none it will be a flat color.'),
  );

  $form['swftools_flowplayer3']['controls']['sliderGradient'] = array(
    '#type' => 'select',
    '#options' => $options['gradient'],
    '#default_value' => $saved_settings['controls']['sliderGradient'],
    '#title' => t('Slider gradient'),
    '#description' => t('The slider can be displayed with a fade that runs vertically, or if set to none it will be a flat color.'),
  );

  $form['swftools_flowplayer3']['controls']['volumeSliderGradient'] = array(
    '#type' => 'select',
    '#options' => $options['gradient'],
    '#default_value' => $saved_settings['controls']['volumeSliderGradient'],
    '#title' => t('Volume slider gradient'),
    '#description' => t('The volume bar can be displayed with a fade that runs vertically, or if set to none it will be a flat color.'),
  );

  $form['swftools_flowplayer3']['controls']['autoHide'] = array(
    '#type' => 'select',
    '#options' => $options['autoHide'],
    '#default_value' => $saved_settings['controls']['autoHide'],
    '#title' => t('Hide control bar'),
    '#description' => t('Specifies whether the controlbar should be hidden when the user is not actively using the player.'),
  );

  $form['swftools_flowplayer3']['controls']['play'] = array(
    '#type' => 'select',
    '#options' => $options['enabled'],
    '#default_value' => $saved_settings['controls']['play'],
    '#title' => t('Play button'),
    '#description' => t('When enabled the play button will be shown. When disabled it will be hidden.'),
  );

  $form['swftools_flowplayer3']['controls']['volume'] = array(
    '#type' => 'select',
    '#options' => $options['enabled'],
    '#default_value' => $saved_settings['controls']['volume'],
    '#title' => t('Volume button'),
    '#description' => t('When enabled the volume button will be shown. When disabled it will be hidden.'),
  );

  $form['swftools_flowplayer3']['controls']['mute'] = array(
    '#type' => 'select',
    '#options' => $options['enabled'],
    '#default_value' => $saved_settings['controls']['mute'],
    '#title' => t('Mute button'),
    '#description' => t('When enabled the mute button will be shown. When disabled it will be hidden.'),
  );

  $form['swftools_flowplayer3']['controls']['time'] = array(
    '#type' => 'select',
    '#options' => $options['enabled'],
    '#default_value' => $saved_settings['controls']['time'],
    '#title' => t('Time display'),
    '#description' => t('When enabled the timer will be shown. When disabled it will be hidden.'),
  );

  $form['swftools_flowplayer3']['controls']['stop'] = array(
    '#type' => 'select',
    '#options' => $options['enabled'],
    '#default_value' => $saved_settings['controls']['stop'],
    '#title' => t('Stop button'),
    '#description' => t('When enabled the stop button will be shown. When disabled it will be hidden.'),
  );

  $form['swftools_flowplayer3']['controls']['playlist'] = array(
    '#type' => 'select',
    '#options' => $options['enabled'],
    '#default_value' => $saved_settings['controls']['playlist'],
    '#title' => t('Playlist buttons'),
    '#description' => t('When enabled the playlist buttons will be shown. When disabled they will be hidden. These buttons will allow navigation through the playlist entries that have been configured for the player. These buttons are always hidden if there are less than two entries in the playlist.'),
  );

  $form['swftools_flowplayer3']['controls']['fullscreen'] = array(
    '#type' => 'select',
    '#options' => $options['enabled'],
    '#default_value' => $saved_settings['controls']['fullscreen'],
    '#title' => t('Fullscreen button'),
    '#description' => t('When enabled the full screen button will be shown. When disabled it will be hidden. Note that to enabled full screen playback the flash settings must also be set to allow full screen, otherwise the button will not work.'),
  );

  $form['swftools_flowplayer3']['controls']['scrubber'] = array(
    '#type' => 'select',
    '#options' => $options['enabled'],
    '#default_value' => $saved_settings['controls']['scrubber'],
    '#title' => t('Scrubber'),
    '#description' => t('The scrubber is the timeline/playhead combination. This specifies whether it should be visible.'),
  );

  $form['swftools_flowplayer3']['controls']['borderRadius'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['controls']['borderRadius'],
    '#title' => t('Border radius'),
    '#description' => t('Specifies the amount of rounding for the control bar. Larger values mean more rounding.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['controls']['scrubberHeightRatio'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['controls']['scrubberHeightRatio'],
    '#title' => t('Scrubber height ratio'),
    '#description' => t('How much the scrubber handle should take of the controlbar total height. A value of 1.0 means that the scrubber handle takes the entire available height in the controlbar plugin.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['controls']['scrubberBarHeightRatio'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['controls']['scrubberBarHeightRatio'],
    '#title' => t('Scrubber bar height ratio'),
    '#description' => t('The ratio between the scrubber bar and the scrubber handle. A value of 1.0 means that the scrubber bar\'s height is the same as the handle\'s height.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['controls']['volumeSliderHeightRatio'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['controls']['volumeSliderHeightRatio'],
    '#title' => t('Volume slider height ratio'),
    '#description' => t('How much the volume slider handle should take of the controlbar total height.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['controls']['volumeBarHeightRatio'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['controls']['volumeBarHeightRatio'],
    '#title' => t('Volume bar height ratio'),
    '#description' => t('The ratio between the volume bar and the voume slider handle. A value of 1.0 means that the bar\'s height is the same as the handle\'s height.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['controls']['timeBgHeightRatio'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['controls']['timeBgHeightRatio'],
    '#title' => t('Time background height ratio'),
    '#description' => t('How much the time/duration display should take of the controlbar total height. A value of 1.0 means that it takes the entire available height of the controlbar plugin.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['controls']['hideDelay'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['controls']['hideDelay'],
    '#title' => t('Hide delay'),
    '#description' => t('The delay in milliseconds before the controlbar is automatically hidden. The timer starts over again when the user stops moving the mouse or moves the mouse out of the player. Specifying 0 here causes the controlbar to hide immediately.'),
    '#size' => 8,
  );

  // Canvas settings
  $form['swftools_flowplayer3']['canvas'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Canvas properties'),
  );

  $form['swftools_flowplayer3']['canvas']['height'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['canvas']['height'],
    '#title' => t('Height'),
    '#description' => t('The height of the player in pixels.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['canvas']['width'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['canvas']['width'],
    '#title' => t('Width'),
    '#description' => t('The width of the player in pixels.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['canvas']['backgroundImage'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['canvas']['backgroundImage'],
    '#title' => t('Background image'),
    '#description' => t('The absolute or relative path to the image that should be used as the background to this plugin. Supported formats are GIF, JPEG and PNG. The syntax is similar to CSS in that you must enclose your path inside a url() wrapper.'),
    '#size' => 24,
  );

  $form['swftools_flowplayer3']['canvas']['backgroundRepeat'] = array(
    '#type' => 'select',
    '#options' => $options['backgroundRepeat'],
    '#default_value' => $saved_settings['canvas']['backgroundRepeat'],
    '#title' => t('Background repeat'),
    '#description' => t('Default is "repeat", which makes the background image repeat both horizontally and vertically. "no-repeat" displays the background image only once from the top left-hand corner.'),
  );

  $form['swftools_flowplayer3']['canvas']['backgroundGradient'] = array(
    '#type' => 'select',
    '#options' => $options['gradient'],
    '#default_value' => $saved_settings['canvas']['backgroundGradient'],
    '#title' => t('Background gradient'),
    '#description' => t('Gradient setting for the canvas.'),
  );

  $form['swftools_flowplayer3']['canvas']['border'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['canvas']['border'],
    '#title' => t('Border'),
    '#description' => t('Draws a border around a plugin\'s edges. The syntax follows the CSS standard: width style color. For example: "1px solid #cccccc". The only supported style currently is "solid", but the width and colour can be set to any valid value.'),
    '#size' => 24,
  );

  $form['swftools_flowplayer3']['canvas']['borderRadius'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['canvas']['borderRadius'],
    '#title' => t('Border radius'),
    '#description' => t('Specifies the amount of rounding for each corner. Larger values mean more rounding.'),
    '#size' => 8,
  );

  // Play button configuration
  $form['swftools_flowplayer3']['play'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Play button properties'),
  );

  $form['swftools_flowplayer3']['play']['url'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['play']['url'],
    '#title' => t('Button url'),
    '#description' => t('A relative path to the play button, which can be a JPG, PNG or SWF file. Note that the button can only be changed in the commercial versions of Flowplayer.'),
    '#size' => 24,
  );

  $form['swftools_flowplayer3']['play']['opacity'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['play']['opacity'],
    '#title' => t('Opacity'),
    '#description' => t('Opacity (0.0-1.0)'),
    '#size' => 5,
  );

  $form['swftools_flowplayer3']['play']['fadeSpeed'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['play']['fadeSpeed'],
    '#title' => t('Fade speed'),
    '#description' => t('Time in milliseconds for the play button to fade out when the video starts.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['play']['rotateSpeed'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['play']['rotateSpeed'],
    '#title' => t('Rotate speed'),
    '#description' => t('The rotation speed of the buffering animation, in milliseconds.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['play']['height'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['play']['height'],
    '#title' => t('Height'),
    '#description' => t('The height of the play button, either in pixels or as a percentage.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['play']['width'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['play']['width'],
    '#title' => t('Width'),
    '#description' => t('The width of the play button, either in pixels or as a percentage.'),
    '#size' => 8,
  );

  $form['swftools_flowplayer3']['play']['label'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['play']['label'],
    '#title' => t('Label'),
    '#description' => t('Text to display with the play button.'),
    '#size' => 25,
  );

  $form['swftools_flowplayer3']['play']['replayLabel'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['play']['replayLabel'],
    '#title' => t('Replay label'),
    '#description' => t('Text to display with the replay button.'),
    '#size' => 25,
  );

  // Logo configuration
  $form['swftools_flowplayer3']['logo'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Logo properties (commercial players only)'),
    '#description' => t('This section is only relevant if you are using a commercial version of Flowplayer.'),
  );

  $form['swftools_flowplayer3']['logo']['use_logo'] = array(
    '#type' => 'checkbox',
    '#default_value' => $saved_settings['logo']['use_logo'],
    '#title' => t('Use logo settings'),
    '#description' => t('Check this option to pass the logo settings to Flowplayer.'),
  );

  $form['swftools_flowplayer3']['logo']['url'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['logo']['url'],
    '#title' => t('URL'),
    '#description' => t('Relative URL of logo image.'),
    '#size' => 100,
  );

  $form['swftools_flowplayer3']['logo']['displayTime'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['logo']['displayTime'],
    '#title' => t('Display time'),
    '#description' => t('mSecs to display logo.'),
    '#size' => 5,
  );

  $form['swftools_flowplayer3']['logo']['fadeSpeed'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['logo']['fadeSpeed'],
    '#title' => t('Fade speed'),
    '#description' => t('Logo fade to opacity 0 speed.'),
    '#size' => 5,
  );

  $form['swftools_flowplayer3']['logo']['opacity'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['logo']['opacity'],
    '#title' => t('Opacity'),
    '#description' => t('Opacity (0.0-1.0)'),
    '#size' => 5,
  );

  $form['swftools_flowplayer3']['logo']['top'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['logo']['top'],
    '#title' => t('Top offset'),
    '#description' => t('Logo position from top.'),
    '#size' => 5,
  );

  $form['swftools_flowplayer3']['logo']['right'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['logo']['right'],
    '#title' => t('Right offset'),
    '#description' => t('Logo position from right.'),
    '#size' => 5,
  );

  $form['swftools_flowplayer3']['logo']['width'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['logo']['width'],
    '#title' => t('Width'),
    '#description' => t('Logo width.'),
    '#size' => 5,
  );

  $form['swftools_flowplayer3']['logo']['height'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['logo']['height'],
    '#title' => t('Height'),
    '#description' => t('Logo height.'),
    '#size' => 5,
  );

  $form['swftools_flowplayer3']['logo']['zIndex'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['logo']['zIndex'],
    '#title' => t('Z-order'),
    '#description' => t('Logo z-order - just leave it as is.'),
    '#size' => 5,
  );

  $form['swftools_flowplayer3']['logo']['fullscreenOnly'] = array(
    '#type' => 'select',
    '#options' => $options['bool'],
    '#default_value' => $saved_settings['logo']['fullscreenOnly'],
    '#title' => t('Fullscreen only'),
    '#description' => t('When set to true the logo is only shown when fullscreen.'),
  );

  $form['swftools_flowplayer3']['logo']['linkUrl'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['logo']['linkUrl'],
    '#title' => t('Link URL'),
    '#description' => t('URL to open if the logo is clicked.'),
    '#size' => 100,
  );

  $form['swftools_flowplayer3']['logo']['linkWindow'] = array(
    '#type' => 'select',
    '#options' => $options['linkWindow'],
    '#default_value' => $saved_settings['logo']['linkWindow'],
    '#title' => t('Link window'),
    '#description' => t('Where should the URL open when the logo is clicked.'),
  );

  $form['swftools_flowplayer3']['accessibility'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Accessibility'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['swftools_flowplayer3']['accessibility']['accessible'] = array(
    '#type' => 'radios',
    '#options' => $options['accessible'],
    '#default_value' => $saved_settings['accessibility']['accessible'],
    '#title' => t('Accessible controls'),
    '#description' => t('This option can be used to enable accessible controls to allow the player to operated via accessible links. The links can be enabled but hidden (but they will be accessible to screen readers), or they can be enabled and displayed below the player.'),
  );
  
  $form['swftools_flowplayer3']['playlists'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Playlists'),
    '#description' => t('Flowplayer 3 uses a JavaScript mechanism to add playlists to the page. This code is currently under active development and is not fully implemented, but it has some basic functionality.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['swftools_flowplayer3']['playlists']['playlist'] = array(
    '#type' => 'radios',
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => $saved_settings['playlists']['playlist'],
    '#title' => t('Create playlist'),
    '#description' => t('Playlists are output to the left of the player. This features requires that Flowplayer 3 embedding is used. This is enabled in the <a href="!url">global Flowplayer 3 settings</a>.', array('!url' => url('admin/settings/swftools/flowplayer3'))),
  );
  
  $form['swftools_flowplayer3']['playlists']['scrollable'] = array(
    '#type' => 'radios',
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => $saved_settings['playlists']['scrollable'],
    '#title' => t('Scrollable playlist'),
    '#description' => t('This feature requires the <a href="http://drupal.org/project/jquery_update">jQuery update</a> module to be installed. The tools.scrollable.js script must also be enabled on the <a href="!url">global Flowplayer 3 settings</a>.', array('!url' => url('admin/settings/swftools/flowplayer3'))),
    '#disabled' => !swftools_flowplayer3_jquery_ok(),
  );
  
  $form['swftools_flowplayer3']['playlists']['images'] = array(
    '#type' => 'radios',
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => $saved_settings['playlists']['images'],
    '#title' => t('Thumbnails'),
    '#description' => t('Whether to include thumbnail images in the playlist.'),
  );
  
  $form['swftools_flowplayer3']['playlists']['style'] = array(
    '#type' => 'radios',
    '#options' => array(
      'gray' => t('Gray'),
      'petrol' => t('Petrol'),
    ),
    '#default_value' => $saved_settings['playlists']['style'],
    '#title' => t('Playlist style'),
    '#description' => t('The default playlist offers two color variants. If a customised playlist template is created then this setting may have no effect.'),
  );
  
  $form['swftools_flowplayer3']['playlists']['fillemptyimages'] = array(
    '#type' => 'radios',
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
    ),
    '#default_value' => $saved_settings['playlists']['fillemptyimages'],
    '#title' => t('Fill empty images'),
    '#description' => t('When enabled then elements in a playlist that do not have a thumbnail will be assigned a default one of a single pixel gif.'),
  );
  
  // If imagecache is available show options, otherwise just store the current values
  if (module_exists('imagecache')) {
    $form['swftools_flowplayer3']['imagecache'] = array(
      '#type'  => 'fieldset',
      '#title' => t('Image cache'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    $options = array(
      SWFTOOLS_UNDEFINED => t('None'),
    );
    
    foreach (imagecache_presets() as $preset) {
      $options[$preset['presetname']] = $preset['presetname'];
    };
    
    $form['swftools_flowplayer3']['imagecache']['imagecache_player'] = array(
      '#type' => 'select',
      '#title' => t('Player splash images'),
      '#default_value' => $saved_settings['imagecache']['imagecache_player'],
      '#options' => $options,
      '#description' => t('Select the image cache preset that should be applied to images used as a splash image in the player.'),
    );
    
    $form['swftools_flowplayer3']['imagecache']['imagecache_playlist'] = array(
      '#type' => 'select',
      '#title' => t('Playlist thumbnails'),
      '#default_value' => $saved_settings['imagecache']['imagecache_playlist'],
      '#options' => $options,
      '#description' => t('Select the image cache preset that should be applied to thumbanils used in playlists.'),
    );
    
  }
  else {
    $form['swftools_flowplayer3']['imagecache']['imagecache_player'] = array(
      '#type' => 'value',
      '#value' => $saved_settings['imagecache']['imagecache_player'],
    );
    $form['swftools_flowplayer3']['imagecache']['imagecache_playlist'] = array(
      '#type' => 'value',
      '#value' => $saved_settings['imagecache']['imagecache_playlist'],
    );
  }

  // Return finished form
  return $form;

}


/**
 * Autocompleter to suggest filename.
 * 
 * @param string $string
 *   The partial name of the file.
 *   
 * @return array
 *   An array of possible matches.
 */
function swftools_flowplayer3_autocomplete_player($string = '') {
  
  // Only do something if there is a string to be matched
  if ($string) {

    // Scan the flowplayer3 directory for possible matches
    $files = file_scan_directory(swftools_get_library('flowplayer3'), '.*');
  
    // Build an array skin names
    $possible_values = array();
    foreach ($files as $file) {
      $possible_values[] = $file->basename;
    }
  
    // Find matches
    $matches = array();  
    foreach ($possible_values as $value) {
      if (preg_match("/$string/i", $value)) {
        $matches[$value] = $value;
      }
    }
    
    print drupal_json($matches);
  } 

}


/**
 * Checks that the jQuery version is 1.3.2. or higher.
 */
function swftools_flowplayer3_jquery_ok() {
  
  // Assume version is < 1.3.2
  $jquery_version_ok = FALSE;
  
  // See if jQuery update is installed, and version level is correct
  if (function_exists('jquery_update_get_version') && version_compare(jquery_update_get_version(), '1.3.2', 'ge')) {
    $jquery_version_ok = TRUE;
  }

  // Return the result
  return $jquery_version_ok;
  
}
