
/**
 * Ensure swftools namespace is established.
 */
var swftools = swftools || {};

/**
* Define Flowplayer functions here.
*/
swftools.flowplayer3 = function() {
  var instances = [];
  var activePlayerID = null;
  var flowplayer_api = false;
    
  function getPlayer (playerID) {
    if (flowplayer_api) {
      return swftools.getObject(playerID + '_api');
    }
    return swftools.getObject(playerID);
  };
  
  return {
    flowplayer3_api_present: function() {
      flowplayer_api = true;
    },
    pushInstance: function(playerID) {
      instances.push(playerID);
    },
    activate: function(playerID) {
      if (activePlayerID && activePlayerID != playerID) {
        getPlayer(activePlayerID).fp_pause();
      };
      activePlayerID = playerID;
    },
    play: function(playerID) {
      getPlayer(playerID).fp_play();
    },
    pause: function(playerID) {
      getPlayer(playerID).fp_pause();
    },
    stop: function(playerID) {
      getPlayer(playerID).fp_stop();
    },
    mute: function(playerID) {
      getPlayer(playerID).fp_mute();
    },
    unmute: function(playerID) {
      getPlayer(playerID).fp_unmute();
    }
  }
}();

Drupal.behaviors.swftools_flowplayer3 = function (context) {
  $('.swftools-flowplayer3:not(.swftools-flowplayer3-processed)', context).addClass('swftools-flowplayer3-processed').each(function () {
    var playerID = $(this).attr('id').substring(9);
    swftools.flowplayer3.pushInstance(playerID);
    if (typeof $f != 'undefined') {
      $f(playerID).onStart(function() {
        swftools.flowplayer3.activate(playerID); 
      });
      $f(playerID).onResume(function() {
        swftools.flowplayer3.activate(playerID); 
      });
    };
  });
  $('[class^=flowplayer3-accessible]:not(.swftools-flowplayer3-processed)', context).addClass('swftools-flowplayer3-processed').each(function () {
    $(this).click(function(){
      var classes = $(this).attr('class');
      var classes = classes.split(' ');
      var parts = classes[0].split('-');
      var idStarts = 24 + parts[2].length;
      var action = "swftools.flowplayer3." + parts[2] + "('" + classes[0].substring(idStarts) + "')";
      eval(action);
      return false;
    });
  });
  if (typeof $f != 'undefined') {
    swftools.flowplayer3.flowplayer3_api_present();
  };
  
  // See if there are any playlists to render
  $('[class^=swftools].clips:not(.swftools-flowplayer3-processed)', context).addClass('swftools-flowplayer3-processed').each(function() {
    var classes = $(this).attr('class');
    var classes = classes.split(' ');
    var playerID = classes[0];
//    flowplayer(playerID).swftools_playlist("div." + playerID + ".clips", {loop:true});
    flowplayer(playerID).swftools_playlist("div." + playerID + ".clips");

    // If there is scrollable playlist set clips height to something very big!
    $('div.swftools-flowplayer3-playlist div.[class^=swftools].clips:not(.swftools-flowplayer3-scrollable)', context).addClass('swftools-flowplayer3-scrollable').css({ height: '20000em', position: 'absolute' });
    
    // Activate scrollable playlists
    $("div.swftools-flowplayer3-playlist").scrollable({
      items:'div.' + playerID + '.clips',
      vertical:true,
      next:'a.down',
      prev:'a.up',
      size: 4
    });
    
  });
    
};

// If flowplayer not yet created we create our own version to handle auto-pause. It will be over-written by flowplayer.js if it is called.
if (typeof flowplayer == 'undefined') {
  var flowplayer = {};
  flowplayer.fireEvent = function() {
      var a = [].slice.call(arguments);
//      console.log(a[1] + ' : ' + a[0]);
      if (a[1] == 'onStart' || a[1] == 'onResume') {
        swftools.flowplayer3.activate(a[0]);
      };
  };
};
