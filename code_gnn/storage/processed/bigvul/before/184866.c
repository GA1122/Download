  bool TranslateInfoBarDelegate::IsTranslatableLanguageByPrefs() {
  Profile* profile =
      Profile::FromBrowserContext(GetWebContents()->GetBrowserContext());
  Profile* original_profile = profile->GetOriginalProfile();
//   TranslateTabHelper* translate_tab_helper =
//       TranslateTabHelper::FromWebContents(GetWebContents());
    scoped_ptr<TranslatePrefs> translate_prefs(
      TranslateTabHelper::CreateTranslatePrefs(original_profile->GetPrefs()));
//       TranslateTabHelper::CreateTranslatePrefs(
//           translate_tab_helper->GetPrefs()));
    TranslateAcceptLanguages* accept_languages =
      TranslateTabHelper::GetTranslateAcceptLanguages(original_profile);
//       translate_tab_helper->GetTranslateAcceptLanguages();
    return translate_prefs->CanTranslateLanguage(accept_languages,
                                                 original_language_code());
  }