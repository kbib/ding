<?php

/**
 * @file
 * Render the library title Panels content type.
 */
?>

<?php if ($header_image): ?>
<div class="picture">
  <div class="picture-inner">
  <?php print $header_image; ?>
  </div>
</div>
<?php endif; ?>

<h1><?php print $library_title; ?></h1>
<?php print $library_navigation; ?>

