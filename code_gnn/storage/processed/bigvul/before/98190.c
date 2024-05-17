void TranslateManager::PageTranslated(TabContents* tab,
                                      PageTranslatedDetails* details) {
  TranslateInfoBarDelegate* infobar;
  if (details->error_type != TranslateErrors::NONE) {
    infobar = TranslateInfoBarDelegate::CreateErrorDelegate(
        details->error_type, tab,
        details->source_language, details->target_language);
  } else if (!IsSupportedLanguage(details->source_language)) {
    UMA_HISTOGRAM_COUNTS("Translate.ServerReportedUnsupportedLanguage", 1);
    infobar = TranslateInfoBarDelegate::CreateErrorDelegate(
        TranslateErrors::UNSUPPORTED_LANGUAGE, tab,
        details->source_language, details->target_language);
  } else {
    infobar = TranslateInfoBarDelegate::CreateDelegate(
        TranslateInfoBarDelegate::AFTER_TRANSLATE, tab,
        details->source_language, details->target_language);
  }
  ShowInfoBar(tab, infobar);
}
