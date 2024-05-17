void TranslateInfoBarDelegate::MessageInfoBarButtonPressed() {
  DCHECK_EQ(translate::TRANSLATE_STEP_TRANSLATE_ERROR, step_);
  if (error_type_ == TranslateErrors::UNSUPPORTED_LANGUAGE) {
    RevertTranslation();
    return;
  }
  TranslateManager* manager =
      TranslateTabHelper::GetManagerFromWebContents(GetWebContents());
  DCHECK(manager);
  manager->TranslatePage(
      original_language_code(), target_language_code(), false);
}
