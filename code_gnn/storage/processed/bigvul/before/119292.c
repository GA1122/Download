void TranslateInfoBarDelegate::Create(bool replace_existing_infobar,
                                      content::WebContents* web_contents,
                                      translate::TranslateStep step,
                                      const std::string& original_language,
                                      const std::string& target_language,
                                      TranslateErrors::Type error_type,
                                      PrefService* prefs,
                                      bool triggered_from_menu) {
  if (step != translate::TRANSLATE_STEP_TRANSLATE_ERROR) {
    DCHECK(TranslateDownloadManager::IsSupportedLanguage(target_language));
    if (!TranslateDownloadManager::IsSupportedLanguage(original_language)) {
      DCHECK(step == translate::TRANSLATE_STEP_TRANSLATING ||
             step == translate::TRANSLATE_STEP_AFTER_TRANSLATE);
      DCHECK_EQ(translate::kUnknownLanguageCode, original_language);
    }
  }

  if ((step == translate::TRANSLATE_STEP_AFTER_TRANSLATE) ||
      (step == translate::TRANSLATE_STEP_TRANSLATING)) {
    TranslateTabHelper* translate_tab_helper =
        TranslateTabHelper::FromWebContents(web_contents);
    if (!translate_tab_helper ||
        translate_tab_helper->GetLanguageState().InTranslateNavigation())
      return;
  }

  infobars::InfoBar* old_infobar = NULL;
  InfoBarService* infobar_service =
      InfoBarService::FromWebContents(web_contents);
  TranslateInfoBarDelegate* old_delegate = NULL;
  for (size_t i = 0; i < infobar_service->infobar_count(); ++i) {
    old_infobar = infobar_service->infobar_at(i);
    old_delegate = old_infobar->delegate()->AsTranslateInfoBarDelegate();
    if (old_delegate) {
      if (!replace_existing_infobar)
        return;
      break;
    }
  }

  scoped_ptr<infobars::InfoBar> infobar(CreateInfoBar(
      scoped_ptr<TranslateInfoBarDelegate>(new TranslateInfoBarDelegate(
          web_contents, step, old_delegate, original_language,
          target_language, error_type, prefs,
          triggered_from_menu))));
  if (old_delegate)
    infobar_service->ReplaceInfoBar(old_infobar, infobar.Pass());
  else
    infobar_service->AddInfoBar(infobar.Pass());
}
