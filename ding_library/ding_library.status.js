// $Id$

/**
 * @file
 * JavaScript behavior to update library open/closed status dynamically.
 */

Drupal.behaviors.dingLibraryStatus = function () {
  var settings = Drupal.settings.officeHours['node' + Drupal.settings.dingLibraryNids[0]];
  $.getJSON(settings.callback + '/' + Drupal.settings.dingLibraryNids.join(',') + '/' + settings.field_name, {}, function (response, textStatus) {
    $.each(response.data, function (nid, hoursData) {
      $('#node-' + nid + ' .library-openstatus')
        // Update the label.
        .text(hoursData.status_local)
        // Remove the existing status classes.
        .removeClass('open')
        .removeClass('closed')
        // Add the current status as a class.
        .addClass(hoursData.status);
    });
  });
};

// Object to hold our globally accessible stuff.
Drupal.dingLibraryStatus = {
  // Refresh library status every 5 minutes.
  'interval': setTimeout(Drupal.behaviors.dingLibraryStatus, 300000)
}

