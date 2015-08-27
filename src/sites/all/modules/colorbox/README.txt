Drupal colorbox module:
------------------------
Maintainers:
  Joe Wheaton (http://drupal.org/user/298179)
  Fredrik Jonsson (http://drupal.org/user/5546)
Requires - Drupal 6 and jQuery Update version 6.x-2.x
License - GPL (see LICENSE)


Overview:
--------
Colorbox is a lightweight customizable lightbox plugin for jQuery
1.3 and 1.4. This module allows for integration of Colorbox into Drupal.
The jQuery library is a part of Drupal since version 5+.

Images, forms, iframed or inline content etc. can be displayed in a
overlay above the current page.

* jQuery - http://jquery.com/
* Colorbox - http://colorpowered.com/colorbox/


Features:
---------

The Colorbox module:

* Excellent integration with Imagefield and Imagecache modules
* Integration with Image module
* Choose between a default style and 5 example styles that are included.
* Style the Colorbox with a custom colorbox.css file in your theme.
* Option to open a login form by clicking on any login link
* Simple API to open any form in a Colorbox
* Drush command to download and install the Colorbox plugin in
  sites/all/libraries

The Colorbox plugin:

* Supports images, image groups, slideshow, ajax, inline, and
  iframed content.
* Appearance is controlled through CSS so users can restyle the box.
* Preloads background images and can preload upcoming images in a
  photo group.
* Generates W3C valid XHTML and adds no JS global variables and
  passes JSLint.
* Tested in Firefox 2 & 3, Safari 3 & 4, Opera 9, Chrome,
  Internet Explorer 6, 7, 8.
* Released under the MIT License.


Installation:
------------
1. Download and install the jQuery Update module version 6.x-2.x.
   Make sure you get the 2.x version since 1.x will not work.
2. Download and unpack the Colorbox plugin in "sites/all/libraries".
   Link: http://colorpowered.com/colorbox/colorbox.zip
   Drush users can use the command "drush colorbox-plugin".
3. Place the Colorbox module directory in your modules folder (this will
   usually be "sites/all/modules/").
4. Go to "Administer" -> "Site building" -> "Modules" and enable the module.

If you want to use Colorbox with the Embedded Media Field module
please check "Enable Colorbox load" in the settings.


Configuration:
-------------
Go to "Administer" -> "Site configuration" -> "Colorbox" to find
all the configuration options.


Use the Views Colorbox Trigger field:
------------------------------------
TODO


Add a custom Colorbox style to your theme:
----------------------------------------
The easiest way is to start with either the default style or one of the
example styles included in the Colorbox JS library download. Simply copy the entire
style folder to your theme and rename it to something logical like "mycolorbox".
Inside that folder are both a .css and .js file, rename both of those as well to match
your folder name: i.e. "colorbox_mycolorbox.css" and "colorbox_mycolorbox.js"

Add entries in your theme's .info file for the Colorbox CSS/JS files:

stylesheets[all][] = mycolorbox/colorbox_mycolorbox.css
scripts[] = mycolorbox/colorbox_mycolorbox.js

Go to "Administer" -> "Site configuration" -> "Colorbox" and select "None" under
"Styles and Options". This will leave the styling of Colorbox up to your theme.
Make any CSS adjustments to your "colorbox_mycolorbox.css" file.


Load content in a Colorbox:
----------------------------------
Check the "Enable Colorbox load" option in Colorbox settings.

This enables custom links that can open content in a Colorbox.
Add the class "colorbox-load" to the link and build the url like this
"[path]?width=500&height=500&iframe=true" or "[path]?width=500&height=500"
if you don't want an iframe.

Other modules may activate this for easy Colorbox integration.


Load inline content in a Colorbox:
----------------------------------
Check the "Enable Colorbox inline"  option in Colorbox settings.

This enables custom links that can open inline content in a Colorbox.
Inline in this context means some part/tag of the current page, e.g. a div.
Replace "id-of-content" with the id of the tag you want to open.

Add the class "colorbox-inline" to the link and build the url like this
"?width=500&height=500&inline=true#id-of-content".

Other modules may activate this for easy Colorbox integration.


Load a selection of forms in a Colorbox:
----------------------------------------
Check the "Enable Colorbox load" option in Colorbox settings.

The following form_id can be used:
* contact_mail_page
* user_login
* user_login_block
* user_register
* user_pass

The links to open a form needs the class "colorbox-load".
The URL should look like this.

"/colorbox/form/[form_id]?destination=[path_to_send_user_to_after_submit]&width=[with_in_pixel]&height=[height_in_pixel]".

Here is an example where the user register form is opened in an
500 by 250 pixel Colorbox.

<a class="colorbox-load" href="/colorbox/form/user_register?destination=user&width=500&height=250">Create new account</a>


Drush:
------
A Drush command is provides for easy installation of the Colorbox
plugin itself.

% drush colorbox-plugin

The command will download the plugin and unpack it in "sites/all/libraries".
It is possible to add another path as an option to the command, but not
recommended unless you know what you are doing.


Example styles borders do not display in Internet Explorer:
----------------------------------------------------------
If you use one of the example styles and have problems with the border
images not loading in Internet Explorer please read
http://colorpowered.com/colorbox/#help_paths.

The default style in Colorbox module does not have this problem.


Contributions:
-------------
* Porting all features from the Thickbox module,
  by Fredrik Jonsson (http://drupal.org/user/5546).
* Image module integration improvements by recrit
  (http://drupal.org/user/452914).
* Help with testing and many good suggestions by Shane
  (http://drupal.org/user/262473).


Last updated:
------------
