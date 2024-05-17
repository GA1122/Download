bool TranslateManager::IsShowingTranslateInfobar(TabContents* tab) {
  return GetTranslateInfoBarDelegate(tab) != NULL;
}
