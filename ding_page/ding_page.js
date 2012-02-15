
/**
 * @file ding_page.js
 * JavaScript behaviors for Ding CMS pages.
 */

if (Drupal.jsEnabled) {
  Drupal.dingPageEdit = {};
  Drupal.dingPageEdit.menuSelection = /^ding_library_(\d+):(\d+)$/

  Drupal.behaviors.dingPageEdit = function() {
    // We're working on the node form here.
    $("#node-form")
    // First hide the library selector.
    .find("#edit-field-library-ref-nid-nid-wrapper").hide().end()
    // Then alter the selected library based on the menu selector.
    .find("#edit-menu-parent")
      .change(function () {
        // Try and figure out the library node ID from the selected menu
        var match = Drupal.dingPageEdit.menuSelection.exec(this.value);
        if (match && match[1]) {
          // If there's a match, select the related library.
          $("#edit-field-library-ref-nid-nid").val(match[1]);
        }
        else {
          // If not, make sure no library is selected.
          $("#edit-field-library-ref-nid-nid").val('');
        }
      })
      // After defining the change event, trigger it, so that me make
      // sure our selection is in sync.
      .change();
  };
}

