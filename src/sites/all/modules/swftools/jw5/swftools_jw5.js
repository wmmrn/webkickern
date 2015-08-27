
/**
 * Ensure swftools namespace is established.
 */
var swftools = swftools || {};

/**
 * Establish jwPlayerReady namespace.
 */
swftools.jwPlayerReady = swftools.jwPlayerReady || {};

/**
 * Callback from LongTail player when an instance is initialized. 
 */
function playerReady(player) {
  swftools.jw5.pushInstance(player.id);
  player = swftools.getObject(player.id);
  
  // Attach listeners and handlers to the player
  jQuery.each(swftools.jwPlayerReady, function() {
    this(player);
  });

};


/**
 * Attach state tracking function to auto-pause players
 */
swftools.jwPlayerReady.swftools_jw5 = function(player) {
  player.addModelListener('STATE','swftools.jw5.stateTracker');
};


/**
* Define LongTail player functions here.
*/
swftools.jw5 = function() {
  var instances = [];
  var activePlayer = null;
  return {
    pushInstance: function(playerID) {
      instances.push(playerID);
    },
    stateTracker: function(player) {
      // States are: IDLE, BUFFERING, PLAYING, PAUSED, COMPLETED
      if (player.newstate == 'PAUSED' || player.newstate == 'COMPLETED') {
        activePlayer = null;
      }
      // Pause other players when another one starts
      if (player.newstate == 'PLAYING') {
        if (activePlayer && player.id != activePlayer) {
          swftools.jw5.pause(activePlayer);
        }
        activePlayer = player.id;
      }
    },
    play: function(playerID) {
      swftools.getObject(playerID).sendEvent('PLAY', 'true');
    },
    pause: function(playerID) {
      swftools.getObject(playerID).sendEvent('PLAY', 'false');
    },
    stop: function(playerID) {
      swftools.getObject(playerID).sendEvent('STOP');
    },
    mute: function(playerID) {
      swftools.getObject(playerID).sendEvent('MUTE', 'true');
    },
    unmute: function(playerID) {
      swftools.getObject(playerID).sendEvent('MUTE');
    }
  }
}();

Drupal.behaviors.swftools_jw5 = function (context) {
  $('[class^=jw5-accessible]:not(.swftools-jw5-processed)', context).addClass('swftools-jw5-processed').each(function () {
    $(this).click(function(){
      var classes = $(this).attr('class');
      var classes = classes.split(' ');
      var parts = classes[0].split('-');
      var idStarts = 16 + parts[2].length;
      var action = "swftools.jw5." + parts[2] + "('" + classes[0].substring(idStarts) + "')";
      eval(action);
      return false;
    });
  });
}
