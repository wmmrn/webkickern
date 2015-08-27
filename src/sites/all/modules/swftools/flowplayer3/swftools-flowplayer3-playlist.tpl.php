<?php

/**
 * @file
 * Template for a Flowplayer 3 playlist.
 */

/**
 * Prepare themeable output for a playlist
 * The markup is placed in a template so that it can be easily over-ridden by the theme system
 * to allow flexibility
 * 
 * This template is quite complex as it must produce the container for the player, the playlist
 * container that will be duplicated, and the JavaScript code. The template has to produce the
 * JavaScript too as the calling module has no way of knowing how the playlist is going to be
 * built, so it cannot produce the required script itself.
 * 
 * Available variables:
 * - $data: The data array that defines this player
 * - $parameters: The parameters to generate a player as part of the $f() call.
 * - $config: The configuration string for the player.
 * - $settings: The settings for this player instance.
 * - $load_player: Whether the load the player immediately (TRUE) or only when the player is activated (FALSE)
 * 
 */
?>

<?php
/**
 * If we are using scrollable playlists then ouptut the additional containers
 */
?>

<?php if ($settings['playlists']['scrollable']) { ?>
<div class="swftools-flowplayer3-playlist-container">
<a class="go up"></a>
<div class="swftools-flowplayer3-playlist">
<?php } ?>


<?php 
/**
 * This markup defines a playlist container - it will be repeated with each playlist element
 */
?>

<div class="<?php print $data['othervars']['id']; ?> clips <?php print $settings['playlists']['style'] == 'gray' ? '' : 'petrol'; ?>">
     
    <!-- single playlist entry as an "template" --> 
    <a href="${url}"> 
        <?php if ($settings['playlists']['images']) { ?> 
          <img class="swftoolsImage" src="" height="46" width="61" alt="" />
        <?php } ?>
        ${title} <span>${artist}</span>
        <em>${duration}</em>
    </a>

</div>


<?php 
/**
 * Close the additional containers we added for scrollable playlists
 */
?>

<?php if ($settings['playlists']['scrollable']) { ?>
</div>
<a class="go down"></a>
</div>
<?php } ?>


<?php 
/**
 * This markup defines the container for the actual player to be placed in
 */
?>

<div id="<?php print $data['othervars']['id']; ?>" class="swftools-flowplayer3-container">

<?php // Only place html_alt on the page if the player is not going to be automatically activated ?>
<?php print $load_player ? '' : $data['othervars']['html_alt']; ?>
</div>


<?php 
/**
 * This markup defines the script to activate the player and populate the playlist
 * Note that we also set the container height and width in here!
 */
?>

<script type="text/javascript">
<!--//--><![CDATA[//><!--
flowplayer("<?php print $data['othervars']['id']; ?>", <?php print $parameters; ?>, <?php print $config; ?>);

$("<?php print $data['othervars']['id']; ?>").height(<?php print $data['othervars']['height']; ?>).width(<?php print $data['othervars']['width']; ?>);
//--><!]]>
</script>
