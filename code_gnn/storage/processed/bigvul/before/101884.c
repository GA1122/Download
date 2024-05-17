void Browser::WorkerCrashed(TabContents* source) {
  TabContentsWrapper* wrapper =
      TabContentsWrapper::GetCurrentWrapperForContents(source);
  wrapper->infobar_tab_helper()->AddInfoBar(new SimpleAlertInfoBarDelegate(
      source, NULL, l10n_util::GetStringUTF16(IDS_WEBWORKER_CRASHED_PROMPT),
      true));
}
