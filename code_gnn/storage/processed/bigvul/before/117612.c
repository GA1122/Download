void BrowserInit::LaunchWithProfile::AddObsoleteSystemInfoBarIfNecessary(
    TabContentsWrapper* tab) {
#if defined(TOOLKIT_USES_GTK)
  if (gtk_check_version(2, 18, 0)) {
    string16 message =
        l10n_util::GetStringFUTF16(IDS_SYSTEM_OBSOLETE_MESSAGE,
                                   l10n_util::GetStringUTF16(IDS_PRODUCT_NAME));
    const char* kLearnMoreURL =
        "http://www.google.com/support/chrome/bin/answer.py?answer=95411";
    InfoBarTabHelper* infobar_helper = tab->infobar_tab_helper();
    infobar_helper->AddInfoBar(
        new LearnMoreInfoBar(infobar_helper,
                             message,
                             GURL(kLearnMoreURL)));
  }
#endif
}
