// $Id$

/**
 * @file
 * JavaScript object to handle our library map.
 */

Drupal.dingLibraryMap = function(mapId, options) {
  this.mapId = mapId;

  this.getMap = function() {
    return Drupal.gmap.getMap(this.mapId);
  };

  this.info = function(mapId, options) {
    this.fullDayNames = options.fullDayNames;
    this.shortDayNames = options.shortDayNames;

    this.init = function() {
      $('.gmap-'+this.mapId+'-gmap').after(
      '<div id="library-info" style="display: none;">'+
        '<div class="frame">'+
          '<div class="inner">'+
            '<h3 class="name"></h3>'+
            '<div class="address">'+
              '<div class="street"></div>'+
              '<div><span class="postal-code"></span> <span class="city"></span></div>'+
            '</div>'+
            '<dl class="opening-hours">'+
            '</dl>'+
          '</div>'+
        '</div>'+
      '</div>');

      var info = this;
      this.getMap().bind('mouseovermarker', function(object) {
        info.showInfo(object);
      });

      $('#library-info').bind('mouseleave', function() {
        info.hideInfo();
      });

      // Triggers for hiding info: mouseout, resize, zoom, move.
      this.getMap().bind('widthchange heightchange zoom move', function() {
        info.hideInfo();
      });
    };

    this.showInfo = function(object) {
      var day, days, section, sectionDays, nextDay, startTime, endTime, startDay, endDay;
      // Add address attributes
      $.each(['name', 'street', 'postal-code', 'city'], function (i, val) {
        $('#library-info .' + val).text(object[val]);
      });

      //Add opening hours
      section = $('#library-info .opening-hours').empty();
      days = ['mon', 'tue', 'wed', 'thu', 'fri', 'sat', 'sun']; //collection of weekdays to use for iteration
      day = 0; //current day index
       //iterate until end of week
      while (day < days.length) {
        sectionDays = [days[day]]; //add current day to to section
        nextDay = day + 1;

        if (object.opening_hours[[days[day]]] &&
            object.opening_hours[[days[day]]].length > 0) {
          startTime = object.opening_hours[[days[day]]][0].open;
          endTime = object.opening_hours[[days[day]]][0].close;

          while ( (nextDay < days.length) &&
                  (object.opening_hours[[days[nextDay]]].length > 0) &&
                  (startTime !== null) && (endTime !== null) &&
                  (startTime == object.opening_hours[[days[nextDay]]][0].open) &&
                  (endTime == object.opening_hours[[days[nextDay]]][0].close)) {
            sectionDays.push(days[nextDay]); //if following days have same hours as current day then add these days to section
            nextDay++;
          }

          if (sectionDays.length > 1) {
            startDay = this.shortDayNames[sectionDays.shift()];
            endDay = this.shortDayNames[sectionDays.pop()];
            sectionDays = startDay.substr(0, 1).toUpperCase()+startDay.substr(1)+'-'+endDay.substr(0, 1).toUpperCase()+endDay.substr(1); //use short day names if section spans more than one day
          }
          else {
            sectionDays = this.fullDayNames[sectionDays.shift()]; //use full day name for section spanning a single day
          }

          startTime = (startTime !== null) ? startTime.substr(0, startTime.lastIndexOf(':')) : '';
          endTime = (endTime !== null) ? endTime.substr(0, endTime.lastIndexOf(':')) : '';

          //add section to opening hours container
          section.append('<dt>'+sectionDays+'</dt>');
          section.append( '<dd>'+
                            startTime +' - '+endTime+
                          '</dd>');
        }

        day = nextDay; //step to day after last day in current section
      }

      //Update css classes to reflect open state
      $('#library-info').removeClass('open').removeClass('closed').addClass(object.state);

      //Add click handler
      $('#library-info').unbind('click').click(function() {
        window.location = object.url;
      });

      //Position and show info
      point = this.getMap().map.fromLatLngToContainerPixel(object.marker.getLatLng());
      $('#library-info').css({ 'left': (point.x-5)+'px', 'top': (point.y- $('#library-info').outerHeight())+'px' }).show();
    };

    this.hideInfo = function() {
      $('#library-info').hide();
    };

    this.init();
  };

  this.resize = function(mapId) {

    this.init = function() {
      $('.gmap-'+this.mapId+'-gmap').after(
        '<a class="resize expand" href="#">'+
          'Expand map'+
        '</a>');

      var resize = this;
      $('a.resize').toggle(function(event) {
        resize.resizeMap(450);
        $(event.target).toggleClass('expand').toggleClass('contract');
      }, function(event) {
        resize.resizeMap(200);
        $(event.target).toggleClass('expand').toggleClass('contract');
      });
    };

    this.resizeMap = function(size) {
      var resize, center;
      this.hideInfo();
      resize = this;
      center = this.getMap().map.getCenter();
      $('.gmap-'+this.mapId+'-gmap').animate({ 'height' : size+'px' }, 1000, 'swing', function() {
        resize.getMap().map.checkResize();
        resize.getMap().map.panTo(center);
      });
    };

    this.init();
  };

  this.goTo = function(mapId, options) {

    this.init = function() {
      var map, lat, long;
      map = this.getMap();

      lat = $.url.param('lat');
      long = $.url.param('long');
      if (lat && long) {
        map.bind('ready', function() {
          map.map.setCenter(new GLatLng(lat, long), 14);
          $.scrollTo(map.map.getContainer(), '500', { offset: -20 });
        });
      }

      $('.link-card a').click(function() {
        geo = $(this).parents('.content:first').find('.geo');
        map.map.setCenter(new GLatLng(geo.find('.latitude').text(), geo.find('.longitude').text()), 14);
        $.scrollTo(map.map.getContainer(), '500', { offset: -20 });
        return false;
      });
    };

    this.init();
  };

  // Initialize the map if Drupal.gmap is available.
  if (Drupal.gmap) {
    this.info(mapId, options);
    this.resize(mapId, options);
    this.goTo(mapId, options);
  }
};

