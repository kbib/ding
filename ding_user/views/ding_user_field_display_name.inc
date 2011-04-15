<?php

/**
 * Field handler to present the Ding! user display name.
 */
class ding_user_field_display_name extends views_handler_field_user_name {

  /**
   * Override init() to add the display name field.
   */
  function init(&$view, &$data) {
    parent::init($view, $data);
    $this->additional_fields['display_name'] = array( 'table' => 'ding_user',
                                                      'field' => 'display_name');
  }

  /**
   * Ovveride query() to avoid adding the non-existing display_name field to the users table.
   * This is added in init().
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }  
  
  function render_link($data, $values) {
    if (!empty($this->options['link_to_user']) || !empty($this->options['overwrite_anonymous'])) {
      $account = new stdClass();
      $account->uid = $values->{$this->aliases['uid']};
      if (!empty($this->options['overwrite_anonymous']) && !$account->uid) {
        // This is an anonymous user, and we're overriting the text.
        return check_plain($this->options['anonymous_text']);
      }
      elseif (!empty($this->options['link_to_user'])) {
        $account->name = $values->{$this->aliases['display_name']};
        return theme('username', $account);
      }
    }
    // Otherwise, there's no special handling, so return the data directly.
    return $values->{$this->aliases['display_name']};
  }

}
  
