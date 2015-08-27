<?php

/**
 * @file
 * Configuration settings for LongTail Image Rotator.
 */

function swftools_imagerotator_admin_form() {

  // Fetch form definition
  $form = swftools_imagerotator_profile_form();
  
  // Add custom form handler to flush cache upon submit
  $form['#submit'][] = 'swftools_admin_settings_submit';
  
  // Return finished form
  return system_settings_form($form);

}


/**
 * Options used to populate the select elements of the form above
 * Collect them here to make them easier to find and adjust!
 */
function _swftools_imagerotator_options() {
  $options['overstretch'] = array('default' => 'default', 'false' => 'false', 'true' => 'true', 'fit' => 'fit', 'none' => 'none', );
  $options['repeat'] = array('default' => 'default', 'false' => 'false', 'true' => 'true', 'list' => 'list', );
  $options['linktarget'] = array('default' => 'default', '_self' => '_self', '_blank' => '_blank', );
  $options['transition'] = array('default' => 'default', 'fade' => 'fade', 'bgfade' => 'bgfade', 'blocks' => 'blocks', 'bubbles' => 'bubbles', 'circles' => 'circles', 'flash' => 'flash', 'fluids' => 'fluids', 'lines' => 'lines', 'slowfade' => 'slowfade', 'random' => 'random', );
  $options['bool'] = array('default' => 'default', 'true' => 'true', 'false' => 'false');
  return $options;
}


/**
 * Returns a form definition for use by the profile system.
 *
 * @return
 *   A form definition array
 */
function swftools_imagerotator_profile_form($profile = '') {

  // Retrieve default or saved settings
  $saved_settings = _swftools_imagerotator_settings($profile, SWFTOOLS_FULL_SETTINGS);

  // Retrieve options needed to populate select elements
  $options = _swftools_imagerotator_options();
  
  // See if colorpicker 2 is loaded
  $colorfield = function_exists('colorpicker_2_or_later') ? 'colorpicker_textfield' : 'textfield';
  
  // Initialise tree for these settings
  $form['swftools_imagerotator']['#tree'] = TRUE;

  $form['swftools_imagerotator']['basic'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Basic'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['swftools_imagerotator']['basic']['height'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['basic']['height'],
    '#size' => 8,
    '#maxlength' => 5,
    '#title' => t('Height'),
    '#description' => t('Leave blank for default. (<em>height</em>)'),
  );
  
  $form['swftools_imagerotator']['basic']['width'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['basic']['width'],
    '#size' => 8,
    '#maxlength' => 5,
    '#title' => t('Width'),
    '#description' => t('Leave blank for default. (<em>width</em>)'),
  );
  
  $form['swftools_imagerotator']['color'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Color'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['swftools_imagerotator']['color']['backcolor'] = array(
    '#type' => $colorfield,
    '#default_value' => $saved_settings['color']['backcolor'],
    '#size' => 8,
    '#maxlength' => 7,
    '#title' => t('Background color'),
    '#description' => t('Enter a hex value eg. for white enter <b>#FFFFFF</b>. (<em>backcolor</em>)'),
  );
  
  $form['swftools_imagerotator']['color']['frontcolor'] = array(
    '#type' => $colorfield,
    '#default_value' => $saved_settings['color']['frontcolor'],
    '#size' => 8,
    '#maxlength' => 7,
    '#title' => t('Text color'),
    '#description' => t('Enter a hex value eg. for white enter <b>#FFFFFF</b>. (<em>frontcolor</em>)'),
  );
  
  $form['swftools_imagerotator']['color']['lightcolor'] = array(
    '#type' => $colorfield,
    '#default_value' => $saved_settings['color']['lightcolor'],
    '#size' => 8,
    '#maxlength' => 7,
    '#title' => t('Rollover color'),
    '#description' => t('Enter a hex value eg. for white enter <b>#FFFFFF</b>. (<em>lightcolor</em>)'),
  );
  
  $form['swftools_imagerotator']['appearance'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Appearance'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['swftools_imagerotator']['appearance']['logo'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['appearance']['logo'],
    '#title' => t('Logo URL'),
    '#description' => t('Full url to logo for a watermark, use PNG files for best results. (<em>logo</em>)'),
  );
  
  $form['swftools_imagerotator']['appearance']['overstretch'] = array(
    '#type' => 'select',
    '#default_value' => $saved_settings['appearance']['overstretch'],
    '#title' => t('Overstretch'),
    '#options' => $options['overstretch'],
    '#description' => t('Defines how to stretch images to fit the display. (<em>overstretch</em>)'),
  );
  
  $form['swftools_imagerotator']['appearance']['showicons'] = array(
    '#type' => 'select',
    '#options' => $options['bool'],
    '#default_value' => $saved_settings['appearance']['showicons'],
    '#title' => t('Show icons'),
    '#description' => t('Show status icons in display. (<em>showicons</em>)'),
  );
  
  $form['swftools_imagerotator']['appearance']['shownavigation'] = array(
    '#type' => 'select',
    '#default_value' => $saved_settings['appearance']['shownavigation'],
    '#options' => $options['bool'],
    '#title' => t('Show navigation'),
    '#description' => t('If false then all navigation controls are hidden. (<em>shownavigation</em>)'),
  );
  
  $form['swftools_imagerotator']['appearance']['transition'] = array(
    '#type' => 'select',
    '#default_value' => $saved_settings['appearance']['transition'],
    '#options' => $options['transition'],
    '#title' => t('Transition effect'),
    '#description' => t('The way the photos change from one to another. (<em>transition</em>)'),
  );
  
  $form['swftools_imagerotator']['playback'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Playback'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['swftools_imagerotator']['playback']['rotatetime'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['playback']['rotatetime'],
    '#size' => 5,
    '#maxlength' => 3,
    '#title' => t('Rotate time'),
    '#description' => t('Number of seconds between image rotations. (<em>rotatetime</em>)'),
  );
  
  $form['swftools_imagerotator']['playback']['shuffle'] = array(
    '#type' => 'select',
    '#options' => $options['bool'],
    '#default_value' => $saved_settings['playback']['shuffle'],
    '#title' => t('Shuffle'),
    '#description' => t('Shuffle media randomly. (<em>shuffle</em>)'),
  );
  
  $form['swftools_imagerotator']['interaction'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Interaction'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['swftools_imagerotator']['interaction']['enablejs'] = array(
    '#type' => 'select',
    '#options' => $options['bool'],
    '#default_value' => $saved_settings['interaction']['enablejs'],
    '#title' => t('Enable JavaScript'),
    '#description' => t('Enable JavaScript interaction with the flash player. (<em>enablejs</em>)'),
  );
  
  $form['swftools_imagerotator']['interaction']['link'] = array(
    '#type' => 'textfield',
    '#default_value' => $saved_settings['interaction']['link'],
    '#title' => t('Link URL'),
    '#description' => t('Web address linked to logo watermark. (<em>link</em>)'),
  );
  
  $form['swftools_imagerotator']['interaction']['linkfromdisplay'] = array(
    '#type' => 'select',
    '#options' => $options['bool'],
    '#default_value' => $saved_settings['interaction']['linkfromdisplay'],
    '#title' => t('Link from display'),
    '#description' => t('Link "Link URL" but with the whole display as the click area. (<em>linkfromdisplay</em>)'),
  );
  
  $form['swftools_imagerotator']['interaction']['linktarget'] = array(
    '#type' => 'select',
    '#default_value' => $saved_settings['interaction']['linktarget'],
    '#options' => $options['linktarget'],
    '#title' => t('Link target'),
    '#description' => t('Target of "Link URL". (<em>linktarget</em>)'),
  );

  // If imagecache is available show options, otherwise just store the current values
  if (module_exists('imagecache')) {
    $form['swftools_imagerotator']['imagecache'] = array(
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
    
    $form['swftools_imagerotator']['imagecache']['imagecache'] = array(
      '#type' => 'select',
      '#title' => t('Image cache'),
      '#default_value' => $saved_settings['imagecache']['imagecache'],
      '#options' => $options,
      '#description' => t('Select the image cache preset that should be applied to images used in this player.'),
    );
    
  }
  else {
    $form['swftools_imagerotator']['imagecache']['imagecache'] = array(
      '#type' => 'value',
      '#value' => $saved_settings['imagecache']['imagecache'],
    );
  }
  
  // Add custom form handler to strip out blank and default settings
  $form['#submit'][] = 'swftools_imagerotator_admin_form_submit';
  
  // Return the form
  return $form;

}


/**
 * Submit handler to filter out form settings that are either empty strings or 'default'
 */

function swftools_imagerotator_admin_form_submit($form, &$form_state) {
  
  // Keep only those items that actually have a value
  foreach($form_state['values']['swftools_imagerotator'] as $key => $settings) {
    if($temp = array_diff($settings, array('', 'default'))) {
      $filtered_settings[$key] = $temp;
    }
  }
  
  // Replace full suite of values with the reduced set we created
  $form_state['values']['swftools_imagerotator'] = $filtered_settings;
  
}
