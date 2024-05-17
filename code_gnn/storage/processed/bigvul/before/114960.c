TranslateInfoBarDelegate* GetTranslateInfoBarDelegate(
    WebContents* web_contents) {
  InfoBarTabHelper* infobar_helper =
      TabContentsWrapper::GetCurrentWrapperForContents(web_contents)->
          infobar_tab_helper();
  for (size_t i = 0; i < infobar_helper->infobar_count(); i++) {
    InfoBarDelegate* infobar = infobar_helper->GetInfoBarDelegateAt(i);
    if (infobar->AsTranslateInfoBarDelegate())
      return infobar->AsTranslateInfoBarDelegate();
  }
  return NULL;
}
