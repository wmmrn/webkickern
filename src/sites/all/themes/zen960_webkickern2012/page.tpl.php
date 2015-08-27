<?php
// $Id:
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>

<body class="<?php print $body_classes; ?>">
  <div id="page" class="container-16 clear-block">
    <div id="site-header" class="grid-16 clear-block">
        <div id="header-inner">
            <div id="user-nav" class="grid-10 alpha">
                <?php print $webkickern_user_nav ?>
                <div id="branding" class="grid-10">
                    <span id="logo"><a href="/<?php print webkickern_yearFromUrl($_GET['q']); ?>">Webickern</a></span>
                </div> <!--/#branding -->
            </div>
            <div id="banner-webmonday" class="grid-2">
                <a href="http://www.wmmrn.de/"><img src="/<?php print path_to_theme() ?>/images/webmonday_pin.png" alt="Webmontag MRN Banner" /></a>
            </div>
            <div id="top-social-links" class="grid-4 omega">
                <div>
                <a href="<?php print url('node/69') ?>"><img alt="Alles für einen guten Zweck" class="icon_heart" src="/<?php print path_to_theme() ?>/images/icon_heart.png" /></a>
                <a href="http://twitter.com/wmmrn"><img class="icon_twitter" alt="@wmmrn" src="/<?php print path_to_theme() ?>/images/icon_twitter.png" /></a>
                <a href="/rss.xml"><img class="icon_rss" alt="RSS" src="/<?php print path_to_theme() ?>/images/icon_rss.png" /></a>
                <a href="http://www.facebook.com/wmmrn?"><img class="icon_fb" alt="Facebook" src="/<?php print path_to_theme() ?>/images/icon_fb.png" /></a>
                </div>
            </div>
        </div> <!--/#header-inner -->
    </div> <!--/#site-header -->

    <div id="site-menu" class="grid-16 clear-block">

        <?php if ($navbar): ?>
          <div id="navbar-inner" class="grid-13 alpha omega">
            <?php print $navbar; ?>
          </div> <!--/#navbar-inner -->
          <div id="year-selector" class="grid-3 clear-block">
            <div><?php print webkickern_yearSelector($_GET['q']) ?></div>
          </div>
        <?php endif; ?>

    </div> <!--/#site-menu -->

    <?php //print ns('grid-16', $left, 4, $right, 3) . ' ' . ns('push-4', !$left, 4); ?>

    <div id="main" class="column grid-16">

        <div id="content">

          <div id="content-inner" class="<?php print (arg(0) == 'admin') ? 'grid-15' : 'grid-10 alpha'; ?>">
          <div id="inner-wrapper">

          <?php //print $breadcrumb; ?>

          <?php if ($title): ?>
            <h1 class="title" id="page-title"><?php print $title; ?></h1>
          <?php endif; ?>

          <?php if ($tabs): ?>
            <div class="tabs"><?php print $tabs; ?></div>
          <?php endif; ?>

          <?php print $messages; ?>
          <?php print $help; ?>

          <?php if ($content_top): ?>
          <div id="content-top" class="region region-content_top">
            <?php print $content_top; ?>
          </div> <!-- /#content-top -->
          <?php endif; ?>

          <?php if (!$is_front): // we don't nedd the content at front ?>
              <div id="main-content" class="region clear-block">
                <?php print $content; ?>
              </div> <!-- /#main-content -->
          <?php endif; ?>

          <?php if ($content_left): ?>
          <div id="contentleft-region" class="grid-5 alpha">
            <div id="contentleft-inner">
            <?php print $content_left; ?>
              </div>
          </div> <!--//end #footer-inner -->
          <?php endif; ?>

          <?php if ($content_right): ?>
          <div id="contentright-region" class="grid-5 omega">
            <div id="contentright-inner">
            <?php print $content_right; ?>
              </div>
          </div> <!--//end #footer-inner -->
          <?php endif; ?>

        <?php if ($content_bottom): ?>
          <div id="content-bottom" class="region region-content_bottom">
            <?php print $content_bottom; ?>
          </div> <!-- /#content-bottom -->
        <?php endif; ?>

        </div>
        </div> <!-- // #content-inner -->
      </div> <!-- //#content -->

      <div id="content-closure" class="grid-16 alpha omega">

      <div id="bottom-logo" class="grid-6 alpha">
        <p>2012 POWERED BY</p>
        <a href="http://www.cyperfection.de/"><img src="/<?php print path_to_theme() ?>/images/logo_cyperfection.png" alt="Powered by cyperfection gmbh""/></a>
        <p>Karl-Krämer-Straße 4<br />67061 Ludwigshafen am Rhein<br />
          <a href="http://www.cyperfection.de" target="_blank">http://www.cyperfection.de</a></p>
      </div>

      <div id="footer-nav">
        <div>
            <?php print $webkickern_footer_nav ?>
        </div>
        <div id="bottom-social-links">
            <a href="http://twitter.com/wmmrn"><img class="icon_twitter" alt="@wmmrn" src="/<?php print path_to_theme() ?>/images/icon_twitter_bottom.jpg" /></a>
            <a href="/rss.xml"><img class="icon_rss" alt="RSS" src="/<?php print path_to_theme() ?>/images/icon_rss_bottom.jpg" /></a>
            <a href="http://www.facebook.com/wmmrn?"><img class="icon_fb" alt="Facebook" style="padding-right:10px;" src="/<?php print path_to_theme() ?>/images/icon_facebook_bottom.jpg" /></a>
        </div>
      </div>

      </div> <!-- /#content-closure -->
      <div id="web-sponsor" class="grid-16 alpha omega">Website sponsored by <a href="http://www.digi-info.de">[di] digitale informationssysteme gmbh</a></div>
    </div> <!-- //end #main -->

    <?php if ($XXXleft): ?>
    <div id="sidebar-left" class="column sidebar region grid-4 <?php print ns('pull-12', $right, 3); ?>">
        <div id="sidebar-left-inner">
        <?php print $left; ?>
      </div>
    </div> <!-- //end #sidebar-left-inner -->
    <?php endif; ?>

    <?php if ($XXXright): ?>
    <div id="sidebar-right" class="column sidebar region grid-3">
        <div id="sidebar-right-inner">
        <?php print $right; ?>
      </div>
    </div> <!--//end #sidebar-right-inner -->
    <?php endif; ?>

    <div id="footer" class="grid-16 clear-block">

      <?php if ($footer): ?>
        <div id="footer-region" class="region grid-16 clear-block">
            <div id="footer-inner">
            <?php print $footer; ?>
          </div>
        </div> <!--//end #footer-inner -->
      <?php endif; ?>

      <?php if ($footer_left): ?>
        <div id="footerleft-region" class="region grid-6 alpha">
            <div id="footerleft-inner">
            <?php print $footer_left; ?>
          </div>
        </div> <!--//end #$footerleft-inner -->
      <?php endif; ?>

      <?php if ($footer_center): ?>
        <div id="footercenter-region" class="region grid-5">
            <div id="footercenter-inner">
            <?php print $footer_center; ?>
          </div>
        </div> <!--//end #footercenter-inner -->
      <?php endif; ?>

      <?php if ($footer_right): ?>
        <div id="footeright-region" class="region grid-5 omega">
            <div id="footeright-inner">
            <?php print $footer_right; ?>
          </div>
        </div> <!--//end #footeright-inner -->
      <?php endif; ?>

      <?php if ($footer_message): ?>
        <div id="footer-message" class="grid-16">
          <?php print $footer_message; ?>
        </div>
      <?php endif; ?>

    </div> <!-- /#footer -->

  </div> <!-- /#page -->


<?php print $closure; ?>

</body>
</html>