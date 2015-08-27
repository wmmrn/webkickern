/**
 * This file is adapted from flowplayer.playlist 3.0.7
 *
 * Author: Tero Piirainen, <info@flowplayer.org>
 * Copyright (c) 2008 Flowplayer Ltd
 *
 * Dual licensed under MIT and GPL 2+ licenses
 * SEE: http://www.opensource.org/licenses
 * 
 * Date: 2009-02-16 06:51:28 -0500 (Mon, 16 Feb 2009)
 * Revision: 1454 
 */ 
(function($) {
  
  $f.addPlugin("swftools_playlist", function(wrap, options) {

    // self points to current Player instance
    var self = this;  
    
    var opts = {
      playingClass: 'playing',
      pausedClass: 'paused',
      progressClass:'progress',
      template: '<a href="${url}">${title}</a>',
      loop: false,
      playOnClick: true,
    };    
    
    var clipEls = [];
    
    $.extend(opts, options);
    wrap = $(wrap);   
    var els = null;
    var playlistLastElement = 0;
    
    
//{{{ "private" functions
        
    function toString(clip) {
      
      // Get the template
      var el = template;
      
      // Iterate over each clip element as key and value
      $.each(clip, function(key, val) { 
        
        // If the value is not a function then try to make a replacement
        if (!$.isFunction(val)) {
          el = el.replace("$\{" +key+ "\}", val).replace("$%7B" +key+ "%7D", val);      
        }
      });
      
      // We have to process images separately to avoid "page not found" errors from the template
      if (typeof clip.swftoolsImage != 'undefined') {
        el = el.replace('class="swftoolsImage" src=""', 'src="' + clip.swftoolsImage + '"');
      };
      
      // Return the completed element ready for inclusion
      return el;
    }
    
    // assign onClick event for each clip
    function bindClicks() {     
      els = wrap.children().unbind("click.playlist").bind("click.playlist", function() {

        // Extract the index of the clip we actually want to play (may be different to DOM index)
        var playMe = $(this).attr('class').split(' ');
        playMe = parseInt(playMe[0].substring(4));
        
        // If this clip is a thumbnail then we want to autoPlay the next one
        // Why can't we do this in buildPlaylist - it ignores the autoPlay property!
        if (typeof self.getClip(playMe).swftoolsThumb != 'undefined') {
          self.getClip(playMe+1).update({autoPlay: true});
        }
         
        // Play the clip that we want
        return play($(this), playMe);
            
      });   
    }
    
    function buildPlaylist() {
      wrap.empty();
      var playMe = null;
      var elCount = 0;
      
      // Iterate over the playlist
      $.each(self.getPlaylist(), function() {  
  
        // Update the size of the playlist
        playlistLastElement = this.index;
        
        // Why does this not work?! It sets true, but buildClicks() doesn't work
//        self.getClip(this.index).update({autoPlay: true});
        
        // If swftoolsThumb is set then this clip is a splash image
        // Do not add to DOM, but store its index for the next clip to use
        if (typeof this.swftoolsThumb != 'undefined') {
          playMe = this.index;
        } 
        else {
          
          // This is a clip to be played - add it to the DOM
          wrap.append(toString(this));
          el = wrap.children(":last");
          
          // Store the DOM element index using clip index as key
          // This is so we know what DOM element to attach classes to later
          clipEls[this.index] = elCount++;
          
          // If playMe is set then we had a splash image - when this playlist
          // entry is clicked we want to play the PREVIOUS clip first to get the splash
          // image on screen. We use the class play[n] to indicate what we want to play
          if (playMe != null) {
            el.addClass('play' + playMe);
            playMe = null;
          }
          
          // If playMe is null then this is straightforward clip - just play it
          else {
            el.addClass('play' + this.index);
          };
        };
      });       
      
      // Attach click behaviors
      bindClicks();
    } 

    
    function play(el, clip)  {
      
      if (el.hasClass(opts.playingClass) || el.hasClass(opts.pausedClass)) {
        self.toggle();
        
      } else {
        el.addClass(opts.progressClass);
        self.play(clip);              
      }     
      
      return false;
    } 
    
    
    function clearCSS() {
      els.removeClass(opts.playingClass);
      els.removeClass(opts.pausedClass);
      els.removeClass(opts.progressClass);      
    }
    
    function getEl(clip) {
      return els.filter(function(index) {
        return index == clipEls[clip.index];
      });
    }
//}}}  
     
    /* setup playlists with onClick handlers */ 
      
    var template = wrap.is(":empty") ? opts.template : wrap.html(); 
    buildPlaylist();      
    
    // When a clip starts attach the playingClass to the DOM element
    self.onBegin(function(clip) {
      // If this clip doesn't have a DOM element then do nothing
      if (typeof clipEls[clip.index] == 'undefined') {
        return;
      }
      // Clear all existing playingClass and set a new one
      clearCSS();   
      getEl(clip).addClass(opts.playingClass);
    }); 
    
    // onPause  
    self.onPause(function(clip) {
      getEl(clip).removeClass(opts.playingClass).addClass(opts.pausedClass);    
    }); 
    
    // onResume
    self.onResume(function(clip) {
      getEl(clip).removeClass(opts.pausedClass).addClass(opts.playingClass);    
    });   
    
    // what happens when clip ends and we're not looping?
    if (!opts.loop) {
      
      // stop the playback if this isn't a splash image
      self.onBeforeFinish(function(clip) {
        if (typeof clip.swftoolsThumb == 'undefined' && clip.index < playlistLastElement) {
          return false;
        };
      });

    };

    // onUnload
    self.onUnload(function() {
      clearCSS();   
    });
    
    // onPlaylistReplace
    self.onPlaylistReplace(function() {
      buildPlaylist();    
    });
    
    // onClipAdd
    self.onClipAdd(function(clip, index) {  
      els.eq(index).before(toString(clip));     
      bindClicks(); 
    });   
    
    return self;
    
  });
    
})(jQuery);   
