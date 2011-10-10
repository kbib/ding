<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function ding_base_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "oembedcore" && $api == "oembedcore") {
    return array("version" => 1);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}
