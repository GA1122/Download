ShowTranslateBubbleResult BrowserView::ShowTranslateBubble(
    content::WebContents* web_contents,
    translate::TranslateStep step,
    translate::TranslateErrors::Type error_type,
    bool is_user_gesture) {
  if (contents_web_view_->HasFocus() &&
      !GetLocationBarView()->IsMouseHovered() &&
      web_contents->IsFocusedElementEditable()) {
    return ShowTranslateBubbleResult::EDITABLE_FIELD_IS_ACTIVE;
  }

  translate::LanguageState& language_state =
      ChromeTranslateClient::FromWebContents(web_contents)->GetLanguageState();
  language_state.SetTranslateEnabled(true);

  if (IsMinimized())
    return ShowTranslateBubbleResult::BROWSER_WINDOW_MINIMIZED;

  toolbar_->ShowTranslateBubble(web_contents, step, error_type,
                                is_user_gesture);
  return ShowTranslateBubbleResult::SUCCESS;
}
