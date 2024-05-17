void ExtensionInstallUI::ShowThemeInfoBar(const std::string& previous_theme_id,
                                          bool previous_using_native_theme,
                                          const Extension* new_theme,
                                          Profile* profile) {
  if (!new_theme->is_theme())
    return;

  Browser* browser = BrowserList::FindTabbedBrowser(profile, true);
  if (!browser)
    return;

  TabContentsWrapper* tab_contents = browser->GetSelectedTabContentsWrapper();
  if (!tab_contents)
    return;
  InfoBarTabHelper* infobar_helper = tab_contents->infobar_tab_helper();

  InfoBarDelegate* old_delegate = NULL;
  for (size_t i = 0; i < infobar_helper->infobar_count(); ++i) {
    InfoBarDelegate* delegate = infobar_helper->GetInfoBarDelegateAt(i);
    ThemeInstalledInfoBarDelegate* theme_infobar =
        delegate->AsThemePreviewInfobarDelegate();
    if (theme_infobar) {
      if (theme_infobar->MatchesTheme(new_theme))
        return;
      old_delegate = delegate;
      break;
    }
  }

  InfoBarDelegate* new_delegate = GetNewThemeInstalledInfoBarDelegate(
      tab_contents, new_theme, previous_theme_id, previous_using_native_theme);

  if (old_delegate)
    infobar_helper->ReplaceInfoBar(old_delegate, new_delegate);
  else
    infobar_helper->AddInfoBar(new_delegate);
}
