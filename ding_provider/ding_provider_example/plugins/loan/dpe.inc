<?php


/**
 * @file
 * Ding loan plugin.
 */

$plugin = array(
  'description' => t('Example loan plugin'),
  'version' => 1,
);

/**
 * Get the users current loans.
 *
 * Should only return data readily available, and leave out data that
 * requires extra requests. Implementation can add extra data used by
 * the 'details' function. All the keys described in the return value
 * can be left to the details function to fill in, so the minimum
 * return value can be:
 * @code
 *   array(
 *     123 => array('id' => 123),
 *   );
 * @endcode
 *
 * @param $account
 *   The Drupal user.
 * @return array
 *   An array of id => loan, where loan is an array of:
 *   - id: The same id as the key.
 *   - materials_number: Materials number, optional. Is shown to the user.
 *   - title: Title of the item. Optional.
 *   - authors: The authors of the item. Optional.
 *   - display_title: Alternative title string, optional. Used if title is
 *     empty. If neither 'title' nor 'display_title' is specified, an
 *     'Title not available' message is displayed.
 *   - loan_date: The date the material was loaned, format YYYY-MM-DD. Optional.
 *   - due_date: The date the material is due, format YYYY-MM-DD. Optional.
 *   - is_renewable: Whether the loan can be renewed.
 *   - remote_loan: Whether it is a remote loan. Optional.
 *   - notes: Additional text associated with the reservation. Optional. 
 *
 * @see ding_provider_example_loan_details()
 */
function ding_provider_example_loan_get_loans($account) {
  $creds = ding_library_user_get_credentials($account);
  if ($creds == DING_PROVIDER_AUTH_REQUIRED) {
    return $creds;
  }
  // As we don't really have a backend, we just fake it.
  return array(
    // The keys should correspond with the items ids.
    '123' => array(
      // Id of the loan/reservation. Used as identifier. No assumptions are
      // made about it, however it should be a sting value suitable for values
      // in forms and the like.
      'id' => '123',
      // Materials number, optional. Is shown to the user.
      'materials_number' => 1238141,
      // Date of the loan, optional. Format: YYYY-MM-DD.
      'loan_date' => '2010-09-15',
      // Due date of the loan, optional. Format: YYYY-MM-DD.
      'due_date' => strftime('%Y-%m-%d', strtotime('yesterday')),
      // Whether this loan is renewable.
      'is_renewable' => TRUE,
      // Whether this is a remote loan.
      'remote_loan' => FALSE,
      // Now, we could return a 'ting_object' here, but for efficiencys sake,
      // we note the id here, and load it in 'details'. For responses without
      // a ting reference, see the next entry.
      'provider_id' => 41406828, // Larry Niven, The Ringworld throne
    ),
    'euaeu' => array(
      'id' => 'euaeu', // string id...
      // Title of the item, optional.
      'title' => 'Ark',
      // Authors of the item, optional. Not shown if display_title is used.
      'authors' => 'Stephen Baxter',
      // Alternative title string, optional. Used if title is empty. If
      // neither 'title' nor 'display_title' is specified, an 'Title not
      // available' message is displayed.
      'display_title' => 'Stephen Baxter: Ark',
      'loan_date' => '2010-09-15',
      'due_date' => strftime('%Y-%m-%d', strtotime('+3 days')),
      'is_renewable' => TRUE,
    ),
    431 => array(
      'id' => 431, // not string for testing purposes
      'provider_id' => 43389122, // Alastair Reynolds, Absolution gap
      'loan_date' => '2010-09-15',
      'due_date' => strftime('%Y-%m-%d', strtotime('today')),
      'is_renewable' => TRUE,
      'remote_loan' => TRUE,
    ),
    '4364625' => array(
      'id' => '4364625',
      'provider_id' => 41168315, // Douglas Adams, The hitch hiker's guide to the galaxy
      // 'loan_date' missing for testing
      'due_date' => strftime('%Y-%m-%d', strtotime('+3 days')),
      'is_renewable' => TRUE,
      'notes' => 'So long and thanks for all the fish!',
    ),
    '4526725' => array(
      'id' => '4526725',
      'provider_id' => 44259362, // Neal Asher, Brass man
      'loan_date' => '2010-09-15',
      'due_date' => strftime('%Y-%m-%d', strtotime('+3 days')),
    ),
    '16164515' => array(
      'id' => '16164515',
      'provider_id' => 42021377, // Iain Banks, Excession
      'loan_date' => '2010-09-15',
      // 'due_date' missing for testing
    ),
  );
}

/**
 * Add details to a loan for detailed display.
 *
 * The 'get loans' callback should only return minimal data, and leave out
 * data that would requiring querying additional services, for
 * speed. 'details' will be called when more data is needed, and should
 * populate extra data for display.
 *
 * Should implement internal per-request caching as it might be called
 * multiple times for the same loan, preferably using ctools_static.
 *
 * @param array $loan
 *   The loan to add details to.
 * @return
 *   The loan with details added.
 */
function ding_provider_example_loan_details($loan) {
  $cache = &ctools_static(__FUNCTION__, array());
  if (!isset($cache[$loan['id']])) {
    $loan['extra'] = 'Tadaaa!';
    if ($loan['provider_id'] and $object = ting_get_object_by_local_id($loan['provider_id'])) {
      $loan['ting_object'] = $object;
    }
    $cache[$loan['id']] = $loan;
  }
  return $cache[$loan['id']];
}

/**
 * Renew loans.
 *
 * This function may return the result in different ways:
 * - A string:
 *   A error message for general errors (timeouts, etc).
 * - An array:
 *   Of $id => $result values. The result is either TRUE for success
 *   or a string with an error message.
 * - An empty array:
 *   All renewals are assumed to be successful.
 * - Anything else:
 *   Is considered an unspecified error and the user is notified.
 *
 * @param object $account
 *   The user for which to renew.
 * @param array $loan_ids
 *   Ids of the loans to renew.
 * @return mixed
 *   Renewal result.
 */
function ding_provider_example_loan_renew_loans($account, $loan_ids) {
  $loans = ding_provider_example_loan_get_loans($account);
  if ($loans == DING_PROVIDER_AUTH_REQUIRED) {
    return $loans;
  }

  $result = array();
  // Most providers wont need this much logic, but just hand the
  // request to the backend and return its responses. But as we don't
  // have a backend...
  foreach ($loan_ids as $id) {
    if (!isset($loans[$id])) {
      $result[$id] = t('Unknown loan');
    }
    elseif ($loans[$id]['is_renewable']) {
      if (strtotime($loans[$id]['due_date']) > $_SERVER['REQUEST_TIME']) {
        $result[$id] = TRUE;
      }
      else {
        $result[$id] = t('Cannot renew due');
      }
    }
    else {
      $result[$id] = t('Cannot renew');
    }
  }

  return $result;
}
