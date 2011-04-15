<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function ding_library_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => 1);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function ding_library_imagecache_default_presets() {
  $items = array(
    '680_200_crop' => array(
      'presetname' => '680_200_crop',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '680',
            'height' => '',
            'upscale' => 1,
          ),
        ),
        '1' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_crop',
          'data' => array(
            'width' => '680',
            'height' => '200',
            'xoffset' => 'center',
            'yoffset' => 'center',
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function ding_library_node_info() {
  $items = array(
    'library' => array(
      'name' => t('Library'),
      'module' => 'features',
      'description' => t('A physical library, with a location, opening hours, etc.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'has_body' => '1',
      'body_label' => t('Description'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_rules_defaults().
 */
function ding_library_rules_defaults() {
  return array(
    'rules' => array(
      'rules_library_staff_profile_deleted' => array(
        '#type' => 'rule',
        '#set' => 'event_node_delete',
        '#label' => 'library_staff_profile_deleted',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'Library',
          '1' => 'ding_library',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'view' => 'library_staff_list',
            ),
            '#name' => 'cache_actions_action_clear_views_cache',
            '#info' => array(
              'label' => 'Clear the cache of a library_staff_list view.',
              'label callback' => FALSE,
              'arguments' => array(
                'view' => array(
                  'type' => 'string',
                  'label' => 'View',
                  'description' => 'Specify the view you want to clear.',
                ),
              ),
              'module' => 'Cache Actions',
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'rules_library_staff_profile_saved' => array(
        '#type' => 'rule',
        '#set' => 'event_node_insert',
        '#label' => 'library_staff_profile_saved',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'Library',
          '1' => 'ding_library',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#info' => array(
              'label' => 'Created content is staff profile',
              'label callback' => FALSE,
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#name' => 'rules_condition_content_is_type',
            '#settings' => array(
              'type' => array(
                'profile' => 'profile',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#type' => 'condition',
            '#weight' => 0,
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'view' => 'library_staff_list',
            ),
            '#name' => 'cache_actions_action_clear_views_cache',
            '#info' => array(
              'label' => 'Clear the cache of a library_staff_list view.',
              'label callback' => FALSE,
              'arguments' => array(
                'view' => array(
                  'type' => 'string',
                  'label' => 'View',
                  'description' => 'Specify the view you want to clear.',
                ),
              ),
              'module' => 'Cache Actions',
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'rules_library_staff_profile_updated' => array(
        '#type' => 'rule',
        '#set' => 'event_node_update',
        '#label' => 'library_staff_profile_updated',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'Library',
          '1' => 'ding_library',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#info' => array(
              'label' => 'Updated content is staff profile',
              'label callback' => FALSE,
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#name' => 'rules_condition_content_is_type',
            '#settings' => array(
              'type' => array(
                'profile' => 'profile',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#type' => 'condition',
            '#weight' => 0,
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'view' => 'library_staff_list',
            ),
            '#name' => 'cache_actions_action_clear_views_cache',
            '#info' => array(
              'label' => 'Clear the cache of a library_staff_list view.',
              'label callback' => FALSE,
              'arguments' => array(
                'view' => array(
                  'type' => 'string',
                  'label' => 'View',
                  'description' => 'Specify the view you want to clear.',
                ),
              ),
              'module' => 'Cache Actions',
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'ding_library_1' => array(
        '#type' => 'rule',
        '#set' => 'event_ding_library_nightly_cron',
        '#label' => 'library_nighly_cron',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'Library',
          '1' => 'ding_library',
          'ding_library' => 'ding_library',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#info' => array(
              'label' => 'Clear the cache of the “library_list” view.',
              'label callback' => FALSE,
              'arguments' => array(
                'view' => array(
                  'type' => 'string',
                  'label' => 'View',
                  'description' => 'Specify the view you want to clear.',
                ),
              ),
              'module' => 'Cache Actions',
            ),
            '#name' => 'cache_actions_action_clear_views_cache',
            '#settings' => array(
              'view' => 'library_list',
            ),
            '#type' => 'action',
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'ding_library_2' => array(
        '#type' => 'rule',
        '#set' => 'event_node_update',
        '#label' => 'library_updated',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'Library',
          'ding_library' => 'ding_library',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#type' => 'condition',
            '#settings' => array(
              'type' => array(
                'library' => 'library',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_condition_content_is_type',
            '#info' => array(
              'label' => 'Updated content is Bibliotek',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#weight' => 0,
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Clear the cache of a specific view.',
              'arguments' => array(
                'view' => array(
                  'type' => 'string',
                  'label' => 'View',
                  'description' => 'Specify the view you want to clear.',
                ),
              ),
              'module' => 'Cache Actions',
            ),
            '#name' => 'cache_actions_action_clear_views_cache',
            '#settings' => array(
              'view' => 'library_list',
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'ding_library_3' => array(
        '#type' => 'rule',
        '#set' => 'event_node_delete',
        '#label' => 'library_deleted',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'Library',
          'ding_library' => 'ding_library',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#type' => 'condition',
            '#settings' => array(
              'type' => array(
                'library' => 'library',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_condition_content_is_type',
            '#info' => array(
              'label' => 'Deleted content is Bibliotek',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#weight' => 0,
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Clear the cache of a specific view.',
              'arguments' => array(
                'view' => array(
                  'type' => 'string',
                  'label' => 'View',
                  'description' => 'Specify the view you want to clear.',
                ),
              ),
              'module' => 'Cache Actions',
            ),
            '#name' => 'cache_actions_action_clear_views_cache',
            '#settings' => array(
              'view' => 'library_list',
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
    ),
  );
}

/**
 * Implementation of hook_views_api().
 */
function ding_library_views_api() {
  return array(
    'api' => '2',
  );
}
