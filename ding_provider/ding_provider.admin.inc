<?php


/**
 * Provides an overview of used plugins, and setting pages.
 */
function ding_provider_admin_form() {
  $plugin_types = _ding_provider_types();
  foreach ($plugin_types as $type => $def) {
    $form[$type] = array(
      '#type' => 'fieldset',
      '#title' => t('@type provider', array('@type' => $def['name'])),
    );

    $plugins = _ding_provider_get_plugins($type);
    $active_plugin = _ding_provider_get_plugin($type);

    $active_value = $active_plugin ? _ding_provider_plugin_description($active_plugin) : t('None');
    if ($active_plugin === FALSE) {
      $active_value = t('<em>Too many providers</em>');
    }
    elseif ($active_plugin) {
      if (ding_provider_supports($active_plugin['plugin type'], 'settings form')) {
        $active_value .= ' ' . l(t('Settings'), 'admin/settings/ding/ding_provider/' . $active_plugin['plugin type'] . '/settings');
      }
    }

    $form[$type]['active'] = array(
      '#type' => 'item',
      '#title' => t('Currently using'),
      '#value' => $active_value,
    );

    if (sizeof($plugins) > 1) {
      if (isset($active_plugin['name'])) {
        unset($plugins[$active_plugin['name']]);
      }
      $form[$type]['inactive'] = array(
        '#type' => 'item',
        '#title' => t('Inactive'),
        '#value' => theme('item_list', array_map('_ding_provider_plugin_description', $plugins)),
      );
    }
  }

  $form['update'] = array(
    '#type' => 'submit',
    '#value' => 'Update',
  );

  return $form;
}

/**
 * Submit function.
 */
function ding_provider_admin_form_submit($form, &$form_state) {
  cache_clear_all("plugins:ding_provider:", 'cache', TRUE);
}

/**
 * Plugin settings page.
 */
function ding_provider_admin_plugin_settings_page($plugin_type) {
  $plugin_types = _ding_provider_types();
  if (!isset($plugin_types[$plugin_type])) {
    return drupal_not_found();
  }

  $active_plugin = _ding_provider_get_plugin($plugin_type);
  if ($active_plugin && ding_provider_supports($active_plugin['plugin type'], 'settings form')) {
    drupal_set_title(t("Settings for %plugin", array('%plugin' => $active_plugin['description'])));
    return ding_provider_get_form($plugin_type, 'settings form');
  }
  else {
    return drupal_not_found();
  }
}
