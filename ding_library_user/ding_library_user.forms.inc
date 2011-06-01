<?php
// $Id$

/**
 * Provider authentication form.
 *
 * Only used when a logged in user requires access to data from the provider
 * and his Alma credentials are not cached.
 *
 * @param array $form_state
 *    Drupal's form state.
 * @param stdClass $account
 *    User account object.
 * @param mixed $explanation
 *    Explanation for the login form. NULL for the default message.
 * @param mixed $redirect
 *    Redirect destination – boolean TRUE for default destination
 *    (user's profile page).
 */
function ding_library_user_authenticate_form($form_state, $account, $explanation=NULL, $redirect=TRUE) {
  $form = array();

  $form['redirect'] = array(
    '#type' => 'value',
    '#value' => $redirect,
  );

  // Strict NULL check to allow an empty explanation string.
  if (is_null($explanation)) {
    $explanation = t('To access your personal information, you need to authenticate.');
  }

  $form['explanation'] = array(
    '#value' => $explanation,
  );

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Borrower card'),
    '#required' => TRUE,
  );

  $form['pass'] = array(
    '#type' => 'password',
    '#title' => t('PIN code'),
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Log in'),
  );

  return $form;
}

/**
 * Validation for authentication form.
 */
function ding_library_user_authenticate_form_validate($form, &$form_state) {
    $auth_res = ding_provider_invoke('user', 'authenticate', $form_state['values']['name'], $form_state['values']['pass']);

  if (!is_array($auth_res) || !isset($auth_res['result'])) {
    watchdog('ding_library_user', 'Provider returned invalid result: @res', array('@res' => print_r($auth_res, TRUE)), WATCHDOG_DEBUG);
    return;
  }

  if ($auth_res['result']) {
    $user = $form_state['values']['account'];
    if (isset($auth_res['creds'])) {
      $_SESSION['ding_library_user_creds'][$user->uid] = $auth_res['creds'];
    }
    form_set_value($form['account'], $user, $form_state);
  }
  else {
    if (isset($auth_res['messages'])) {
      foreach ($auth_res['messages'] as $message) {
        form_error($form['name'], $message);
      }
    }
  }
}

/**
 * Submit handler for authentication form.
 */
function ding_library_user_authenticate_form_submit($form, &$form_state) {
  // If we do not have a destination already, use the redirect value.
  if (!isset($_REQUEST['destination']) && $form_state['values']['redirect']) {
    // If redirect is boolean TRUE, use the default redirect.
    if (is_bool($form_state['values']['redirect'])) {
      $form_state['redirect'] = 'user/' . $form_state['values']['account']->uid . '/profile';
    }
    else {
      // Otherwise, assume that the redirect value is a sane destination.
      $form_state['redirect'] = $form_state['values']['redirect'];
    }
  }
}

/**
 * Change password form.
 */
function ding_library_user_change_password_form($form_state, $account) {
  $password_length = variable_get('ding_library_user_password_length', 4);
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['current_password'] = array(
    '#type' => 'password',
    '#title' => t('Current password'),
    '#required' => TRUE,
    '#maxlength' => $password_length,
    '#size' => $password_length,
  );

  $form['new_password'] = array(
    '#type' => 'password',
    '#title' => t('New password'),
    '#required' => TRUE,
    '#maxlength' => $password_length,
    '#size' => $password_length,
  );

  $form['new_password_confirm'] = array(
    '#type' => 'password',
    '#title' => t('Repeat new password'),
    '#required' => TRUE,
    '#maxlength' => $password_length,
    '#size' => $password_length,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save new password'),
  );

  return $form;
}

/**
 * Validation for library password changing form.
 */
function ding_library_user_change_password_form_validate($form, &$form_state) {
  $password_length = variable_get('ding_library_user_password_length', 4);
  $creds = ding_library_user_get_credentials($form_state['values']['account']);

  // This should be fixed.
  $old_pass = NULL;
  if ($creds['pass']) {
    $old_pass = $creds['pass'];
  }
  elseif ($creds['password']) {
    $old_pass = $creds['password'];
  }

  if ($form_state['values']['current_password'] != $old_pass) {
    form_set_error('current_password', t('Please provide your current password.'));
  }

  if ($form_state['values']['current_password'] == $form_state['values']['new_password']) {
    form_set_error('new_password', t('The new password must be different than the current password.'));
  }

  if ($form_state['values']['new_password_confirm'] != $form_state['values']['new_password']) {
    form_set_error('new_password_confirm', t('Please input your new password twice for confirmation.'));
  }

  if (!ctype_digit($form_state['values']['new_password']) || strlen($form_state['values']['new_password']) != $password_length) {
    form_set_error('new_password', t('The new password must be @num numerical digits.', array('@num' => $password_length)));
  }
}

/**
 * Submit handler for the password changing form.
 */
function ding_library_user_change_password_form_submit($form, &$form_state) {
  if ($res = ding_provider_invoke('user', 'password change', $form_state['values']['account'], $form_state['values']['new_password']) && $res != DING_PROVIDER_AUTH_REQUIRED) {
    // Cache the new password
    if ($_SESSION['ding_library_user_creds'][$form_state['values']['account']->uid]['pass']) {
      $_SESSION['ding_library_user_creds'][$form_state['values']['account']->uid]['pass'] = $form_state['values']['new_password'];
    }
    elseif ($_SESSION['ding_library_user_creds'][$form_state['values']['account']->uid]['password']) {
      $_SESSION['ding_library_user_creds'][$form_state['values']['account']->uid]['password'] = $form_state['values']['new_password'];
    }

  }
  elseif ($res == DING_PROVIDER_AUTH_REQUIRED) {
    drupal_set_message(t("You need to be logged in to change password."), 'error');
  }
  else {
    drupal_set_message(t("There was a problem changing your password, please contact your library."), 'error');
  }
  // If we do not have a destination already, go to the profile.
  if (!isset($_REQUEST['destination'])) {
    $form_state['redirect'] = 'user/' . $form_state['values']['account']->uid . '/profile';
  }
}

/**
 * Loan details form.
 *
 * @todo Way too much logic in the theme function. Move some of it here.
 */
function ding_library_user_loan_list_form($form_state, $account, $records) {
  $grouped = _ding_library_user_group_loans($records);
  $renew_buttons = FALSE;
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['loan_data'] = array(
    '#type' => 'value',
    '#grouped' => $grouped,
    '#value' => $records,
  );

  $form['loans'] = array(
    '#tree' => TRUE,
  );

  foreach ($records as $loan) {
    $form['loans'][$loan['id']] = array(
      '#type' => 'checkbox',
      '#return_value' => $loan['id'],
      '#disabled' => TRUE,
      '#disabled_text' => t('Unable to renew loan'),
    );
    if ($loan['is_renewable']) {
      $form['loans'][$loan['id']]['#disabled'] = FALSE;
      $renew_buttons = TRUE;
    }
  }

  if ($renew_buttons) {
    $form['buttons']['renew'] = array(
      '#type' => 'submit',
      '#value' => t('Renew selected'),
    );

    $form['buttons']['renew_all'] = array(
      '#type' => 'submit',
      '#value' => t('Renew all'),
      '#attributes' => array('class' => 'renew-all-button'),
    );
  }

  return $form;
}

/**
 * Loan details submit handler.
 */
function ding_library_user_loan_list_form_submit($form, $form_state) {
  global $user;
  $selected = array();

  switch ($form_state['clicked_button']['#parents'][0]) {
    case 'renew':
      foreach ($form_state['values']['loans'] as $key => $value) {
        if ($value) {
          $selected[] = $key;
        }
      }
      break;
    case 'renew_all':
      foreach ($form_state['values']['loan_data'] as $loan) {
        if ($loan['is_renewable']) {
          $selected[] = $loan['id'];
        }
      }
      break;
  }

  $result = ding_provider_invoke('loan', 'renew loans', $user, $selected);
  if (is_array($result)) {
    if (sizeof($result)) {
      $types = array();
      // Flip the array so the keys are the message and the value the affected
      // ids.
      foreach ($result as $id => $res) {
        $types[$res][] = $id;
      }
      // TODO: Add in material titles, but that requires that
      // ding_library_user_loan_list_form does the
      // ding_provider_invoke('loan', 'details') thing before inserting the
      // loans in the form, rather than the theming.
      //
      // Alternatively (or combined), an item_name function that attempts to
      // use either ting_object->title, title, display_title, material_id or
      // id to construct a name for the item.
      foreach ($types as $res => $ids) {
        if (is_string($res)) {
          drupal_set_message(format_plural(sizeof($ids), '@message', '@count materials: @message', array('@message' => $res)), 'error');
        }
        else {
          drupal_set_message(format_plural(sizeof($ids), 'Renewed 1 material.', 'Renewed @count materials.'));
        }
      }
    }
    else {
      drupal_set_message(t('Materials renewed'));
    }
  }
  elseif (is_string($result)) {
    drupal_set_message($result, 'error');
  }
  else {
    drupal_set_message(t('Error renewing materials'), 'error');
  }
}

/**
 * Helper function to group loans by their due dates.
 */
function _ding_library_user_group_loans($records) {
  $groups = array();
  $today = date('Y-m-d', $_SERVER['REQUEST_TIME']);

  foreach ($records as $loan) {
    if ($loan['due_date'] < $today) {
      $groups['overdue'][$loan['id']] = $loan['id'];
    }
    elseif ($loan['due_date'] == $today) {
      $groups['due'][$loan['id']] = $loan['id'];
    }
    else {
      $groups[$loan['due_date']][$loan['id']] = $loan['id'];
    }
  }
  return $groups;
}

