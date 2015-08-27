
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
  swftools.wijering4.pushInstance(player.id);
  player = swftools.getObject(player.id);
  
  // Attach listeners and handlers to the player
  jQuery.each(swftools.jwPlayerReady, function() {
    this(player);
  });

};


/**
 * Attach state tracking function to auto-pause players
 */
swftools.jwPlayerReady.swftools_wijering4 = function(player) {
  player.addModelListener('STATE','swftools.wijering4.stateTracker');
};


/**
* Define LongTail player functions here.
*/
swftools.wijering4 = function() {
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
          swftools.wijering4.pause(activePlayer);
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

Drupal.behaviors.swftools_wijering4 = function (context) {
  $('[class^=wijering4-accessible]:not(.swftools-wijering4-processed)', context).addClass('swftools-wijering4-processed').each(function () {
    $(this).click(function(){
      var classes = $(this).attr('class');
      var classes = classes.split(' ');
      var parts = classes[0].split('-');
      var idStarts = 22 + parts[2].length;
      var action = "swftools.wijering4." + parts[2] + "('" + classes[0].substring(idStarts) + "')";
      eval(action);
      return false;
    });
  });
}
