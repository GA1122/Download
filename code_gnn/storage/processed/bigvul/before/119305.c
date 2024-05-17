bool TranslateInfoBarDelegate::ShouldShowAlwaysTranslateShortcut() {
  DCHECK_EQ(translate::TRANSLATE_STEP_BEFORE_TRANSLATE, step_);
  return !GetWebContents()->GetBrowserContext()->IsOffTheRecord() &&
      (prefs_->GetTranslationAcceptedCount(original_language_code()) >=
          kAlwaysTranslateMinCount);
}
