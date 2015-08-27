<?php
// $Id: drupalcenter.profile,v 1.3.2.1 2007/04/07 09:11:26 Bjoern (bv, bjoern@drupalcenter.de) Exp $

/**
 * The modules that are enabled when this profile is installed.
 *
 * @return
 *  An array of modules to be enabled.
 */
function drupalcenter_profile_modules() {
  $core_optional = array('block', 'color', 'comment', 'filter', 'help', 'menu', 'node', 'system', 'taxonomy', 'user','dblog');
  $contrib = array();
  return array_merge($core_optional, $contrib);
}

/**
 * Implementation of hook_profile_details().
 *
 * This contains an array of profile details for display from the main selection screen.
 */
function drupalcenter_profile_details() {
  return array(
  'name' => 'deutschsprachige Drupal-Version (Drupalcenter-Version)',
  'description' => 'Dieses Profil bietet die M&ouml;glichkeit eine deutschsprachige Drupal-Version mit den Standardeinstellungen zu installieren.'
  );
}

function drupalcenter_profile_tasks() {
 $types = array(
    array(
      'type' => 'page',
      'name' => st('Page'),
      'module' => 'node',
      'description' => st("A <em>page</em>, similar in form to a <em>story</em>, is a simple method for creating and displaying information that rarely changes, such as an \"About us\" section of a website. By default, a <em>page</em> entry does not allow visitor comments and is not featured on the site's initial home page."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    ),
    array(
      'type' => 'story',
      'name' => st('Story'),
      'module' => 'node',
      'description' => st("A <em>story</em>, similar in form to a <em>page</em>, is ideal for creating and displaying content that informs or engages website visitors. Press releases, site announcements, and informal blog-like entries may all be created with a <em>story</em> entry. By default, a <em>story</em> entry is automatically featured on the site's initial home page, and provides the ability to post comments."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    ),
  );

  foreach ($types as $type) {
    $type = (object) _node_type_set_defaults($type);
    node_type_save($type);
  }
  
  // Default page to not be promoted and have comments disabled.
  variable_set('node_options_page', array('status'));
  variable_set('node_options_story', array('status'));
  variable_set('comment_page', COMMENT_NODE_DISABLED);
  //variable_set('comment_story', COMMENT_NODE_DISABLED);


  global $install_locale;
  if ($install_locale == 'de') {
    variable_set('configurable_timezones', '1');
    variable_set('date_default_timezone', '3600');
    variable_set('date_first_day', '1');
    variable_set('date_format_long', 'l, j. F Y - G:i');
    variable_set('date_format_medium', 'j. F Y - G:i');
    variable_set('date_format_short', 'd.m.Y - H:i');
  }
  
  // Update the menu router information.
  menu_rebuild();

  // Footermessage
  variable_set('site_footer', 'Deutschsprachige Drupal Version:  <a href="http://www.drupalcenter.de">Drupal Center</a>');

}
