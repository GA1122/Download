void Browser::JSOutOfMemoryHelper(TabContents* tab) {
  TabContentsWrapper* tcw = TabContentsWrapper::GetCurrentWrapperForContents(
      tab);
  if (tcw) {
    tcw->infobar_tab_helper()->AddInfoBar(new SimpleAlertInfoBarDelegate(
        tab, NULL, l10n_util::GetStringUTF16(IDS_JS_OUT_OF_MEMORY_PROMPT),
        true));
  }
}
