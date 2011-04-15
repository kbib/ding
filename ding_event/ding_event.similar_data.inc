<?php

/**
 * @file ding_event.similar_data.inc
 * Generates the data that is used when looking for similar events.
 */

/**
 * Generate a large array of data, making it easy to find similar events.
 */
function ding_event_similar_data_generate() {
  $data = array(
    'events' => array(),
    'terms' => array(),
    'libraries' => array(),
  );

  // Get information about the datetime field from CCK, so we can make
  // SQL queries against it.
  $datetime_field = content_fields('field_datetime', 'event');
  $db_info = content_database_info($datetime_field);

  // Get events that has not started yet.
  $query = db_query('SELECT n.nid FROM %s JOIN {node} n USING (nid) WHERE n.status = 1 AND %s > NOW() ORDER BY %s ASC', array($db_info['table'], $db_info['columns']['value']['column'], $db_info['columns']['value']['column']));
  while ($nid = db_result($query)) {
    $data['events'][$nid] = $nid;
  }

  // Get Taxonomy term associations.
  $event_count = count($data['events']);
  if ($event_count) {
    $query = db_query('SELECT nid, tid FROM {term_node} WHERE nid IN (' . implode(', ', $data['events']) . ');');
    while ($row = db_fetch_array($query)) {
      $data['terms'][$row['tid']][$row['nid']] = $row['nid'];
    }
  }

  // Get library associations.
  if ($event_count) {
    $library_rel_field = content_fields('field_library_ref', 'event');
    $lr_info = content_database_info($library_rel_field);
    $col_name = $lr_info['columns']['nid']['column'];
    $query = db_query('SELECT nid, %s FROM %s WHERE nid IN (' . implode(', ', $data['events']) . ');', array(
      ':column' => $col_name,
      ':table' => $lr_info['table'],
    ));
    while ($row = db_fetch_array($query)) {
      $data['libraries'][$row[$col_name]][$row['nid']] = $row['nid'];
    }
  }

  return $data;
}

