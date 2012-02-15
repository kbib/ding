
/**
 * Javascript helpers for Ding cart interaction buttons.
 */
Drupal.behaviors.dingCartButtons = function () {
  Drupal.tingButtons.dialogButton(".add-to-cart a", {
    buttons: function(buttons, event, data) {
      if (data.status === 'success') {
        buttons[Drupal.t('View cart\u2026')] = function () {
          window.location = data.cart_link;
        };

        var $count = $('#account-profile .cart .count');
        $count.text(parseInt($count.text(), 10) + 1);
      }
    }
  });
}
