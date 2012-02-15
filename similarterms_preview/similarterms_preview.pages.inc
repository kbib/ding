<?php

/**
 * Get similar terms preview asyncroneously.
 */
function similarterms_preview_page_ahah() {
  $vocabulary = taxonomy_vocabulary_load(intval($_REQUEST['vocabulary_id']));
  $typed_terms = drupal_explode_tags($_REQUEST['terms']);
  $terms = array();
  $result = array();

  if ($vocabulary && !empty($typed_terms)) {
    foreach ($typed_terms as $typed_term) {
      // See if the term exists in the chosen vocabulary
      // and return the tid; otherwise, add a new record.
      $possibilities = taxonomy_get_term_by_name($typed_term);
      foreach ($possibilities as $possibility) {
        if ($possibility->vid == $vocabulary->vid) {
          $terms[$possibility->tid] = $possibility;
        }
      }
    }

    $count = variable_get('simterms_count_'. $vocid, 5);

    if (!empty($terms)) {
      $sql = "
        SELECT n.nid, n.title, COUNT(n.nid) AS ncount
        FROM {node} n
        INNER JOIN {term_node} tn ON n.nid = tn.nid
        WHERE tn.tid IN (" . implode(',', array_keys($terms)) . ")
        AND n.status = 1 AND n.moderate = 0
        GROUP BY n.nid, n.title, n.created
        ORDER BY ncount DESC, n.created DESC
        LIMIT %d";
      $query = db_query($sql, $count);
      while ($row = db_fetch_array($query)) {
        $result['links'][$row['nid']] = l($row['title'], 'node/' . $row['nid']);
      }
    }
  }

  // If the browser wants JSON, let 'em have it.
  if (strpos($_SERVER['HTTP_ACCEPT'], 'application/json') ==! FALSE || strpos($_SERVER['HTTP_ACCEPT'], 'text/javascript') ==! FALSE) {
    drupal_set_header('Content-Type: application/json; charset=utf-8');
    echo json_encode($result);
    return;
  }
  else {
    drupal_set_header('HTTP/1.1 406 Not Acceptable');
    echo 'This method only serves application/json. Please specify that in your accept header if you want any.';
  }
}

