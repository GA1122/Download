void Browser::HideValidationMessage(content::WebContents* web_contents) {
  if (validation_message_bubble_)
    validation_message_bubble_->CloseValidationMessage();
}
