// $Id$

Drupal.behaviors.dingAdminNodeForm = function () {
  // When node title changes, set the menu title accordingly if it has
  // not been filled out yet.
  $("#edit-menu-parent").change(function () {
    if ($(this).val() == 0) {
      $("#edit-menu-link-title").val("");
    }
    else if ($("#edit-menu-link-title").val() == "") {
      $("#edit-menu-link-title").val($("#edit-title").val());
    }
  });
};

