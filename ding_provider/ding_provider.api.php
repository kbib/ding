<?php


/**
 * @file
 * Documentation for ding_provider hooks and pseudo-hooks.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implemented by provider modules to announce their services.
 */
function hook_ding_provider_info() {
  return array(
    'provider_module' => array(
      'name' => 'Administrative title',
    ),
  );
}

/**
 * @} End of "addtogroup hooks".
 */
