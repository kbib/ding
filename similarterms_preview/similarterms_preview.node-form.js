
Drupal.similartermsPreview = {
  // Check interval needs to be fairly high, since Drupal's autocomplete
  // triggers a lot of focus/blur events when working. We need to delay
  // the check so that it doesn't fire before its time.
  'checkInterval': 1500,
  'previousInput': {}
};

/**
 * Check to see if we need to update our preview.
 */
Drupal.similartermsPreview.check = function () {
  $.each(Drupal.settings.similartermsPreview.taggingVocabularies, function () {
    var inputField = $("#edit-taxonomy-tags-" + this);
    var textInput = inputField.val();

    // If the value has truthy and has changed since last check 
    if (textInput && Drupal.similartermsPreview.previousInput[this] != textInput) {
      Drupal.similartermsPreview.update(this, inputField, textInput);
      // Store the text input, so preview won't be refreshed again
      // before the input is changed.
      Drupal.similartermsPreview.previousInput[this] = textInput;
    }
  });
}

/**
 * Update the preview via Drupal's callback.
 */
Drupal.similartermsPreview.update = function (vocabulary_id, inputField, textInput) {
  $.post(Drupal.settings.similartermsPreview.callback, {
    'vocabulary_id': vocabulary_id,
    'terms': textInput
  }, function (data) {
    if (data) {
      // If it does not exist already, create the list for containing the
      // preview list.
      if ($("#similarterms-preview-list-" + vocabulary_id).length == 0) {
        inputField.parent()
          .append('<h4>' + Drupal.t('Similar content') + '</h4>')
          .append('<ul id="similarterms-preview-list-' + vocabulary_id + '" class="similarterms-preview"></ul>');
      }
      // Otherwise, just empty the existing one.
      else {
        $("#similarterms-preview-list-" + vocabulary_id).empty();
      }

      // Finally, add a list item for each returned link.
      $.each(data.links, function () {
        $("#similarterms-preview-list-" + vocabulary_id)
          .append('<li>' + this + '</li>');
      });
    }
  }, 'json');
};

/**
 * Attach our event handlers and set up timers.
 */
$(function(){
  // Check for changes every 500 ms.
  var checkTimerId = setInterval(Drupal.similartermsPreview.check, Drupal.similartermsPreview.checkInterval);
  // As well as right now.
  Drupal.similartermsPreview.check();

  $.each(Drupal.settings.similartermsPreview.taggingVocabularies, function () {
    // Disable the timer when one of the fields has focus.
    $("#edit-taxonomy-tags-" + this)
      .focus(function () { clearTimeout(checkTimerId); })
      .blur(function () { setInterval(Drupal.similartermsPreview.check, Drupal.similartermsPreview.checkInterval); });
  });
});

