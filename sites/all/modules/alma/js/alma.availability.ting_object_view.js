/**
 * @file
 * Specific availability behavior for detailed object page.
 */

Drupal.behaviors.almaAvailabilityTingObjectView = function () {
  // Use get_details to load detailed data for each item on the page.
  Drupal.almaAvailability.get_details(function (data, textStatus) {
    // Update the standard status messages.
    Drupal.almaAvailability.updateStatus(data);

    // Inject data into the list of library that has the item available. 
    if ($("#ting-object .alma-availability").length > 0) {
      $("#ting-object .alma-availability ul.library-list").empty();
      // Iterate over each Alma item's data.
      $.each(data, function (itemIndex, itemData) {
        var container;
        container = $('#ting-item-' + itemData.alma_id + ' .alma-availability ul.library-list');

        // Find holdings, unique by library name.
        $.each(itemData.holdings, function (holdingIndex, holdingData) {
          // If the total count for the library is bigger than the
          // number that library has checked out, it is interpreted as
          // if the item is available.
          // This because available_count is really the number available
          // for reservation, and doesn't count things that can be
          // loaned, but not reserved (14 day loans, etc.).
          if (holdingData.total_count > holdingData.checked_out_count) {
            container.append('<li>' + Drupal.almaAvailability.formatHolding(itemData, holdingData)  + '</li>');
          }
        });
      });
    }
    else {
      // Remove container as no data was received (when displaying one ting object only)
      var container = $('.alma-availability ul.library-list');
      if (container.length === 1) {
        container.parent().remove();
      }
    }
  });
};

