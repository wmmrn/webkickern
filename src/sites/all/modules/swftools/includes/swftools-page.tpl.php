<?php

/**
 * @file
 * Template for a basic page served from the SWF Tools cache.
 * 
 * This template is used when accessing a piece of Flash content via something
 * like LightBox. The content is rendered on to a very basic page that is just
 * sufficient to get the Flash content displayed.
 * 
 * The pre-processor function for this page is just template_preprocess_page(),
 * so everything that is available in page.tpl.php is available here.
 * 
 * We use simplified markup to output only the css and js, and actual swf content, which is in
 * $content.
 * 
 * @see page.tpl.php
 * @see template_process()
 * @see template_preprocess_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body>
<!-- begin SWF Tools content -->
<?php print $content; ?>

<!-- end SWF Tools content -->
<?php print $closure; ?>
</body>
</html>
