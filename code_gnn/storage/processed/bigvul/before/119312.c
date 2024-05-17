TranslateInfoBarDelegate::TranslateInfoBarDelegate(
    content::WebContents* web_contents,
    translate::TranslateStep step,
    TranslateInfoBarDelegate* old_delegate,
    const std::string& original_language,
    const std::string& target_language,
    TranslateErrors::Type error_type,
    PrefService* prefs,
    bool triggered_from_menu)
    : infobars::InfoBarDelegate(),
      step_(step),
      background_animation_(NONE),
      ui_delegate_(TranslateTabHelper::FromWebContents(web_contents),
                   TranslateTabHelper::GetManagerFromWebContents(web_contents),
                   original_language,
                   target_language),
      error_type_(error_type),
      prefs_(TranslateTabHelper::CreateTranslatePrefs(prefs)),
      triggered_from_menu_(triggered_from_menu) {
  DCHECK_NE((step_ == translate::TRANSLATE_STEP_TRANSLATE_ERROR),
            (error_type_ == TranslateErrors::NONE));

  if (old_delegate && (old_delegate->is_error() != is_error()))
    background_animation_ = is_error() ? NORMAL_TO_ERROR : ERROR_TO_NORMAL;
}
