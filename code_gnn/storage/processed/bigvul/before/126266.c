void Browser::WorkerCrashed(WebContents* source) {
  InfoBarTabHelper* infobar_helper =
      InfoBarTabHelper::FromWebContents(source);
  infobar_helper->AddInfoBar(new SimpleAlertInfoBarDelegate(
      infobar_helper,
      NULL,
      l10n_util::GetStringUTF16(IDS_WEBWORKER_CRASHED_PROMPT),
      true));
}
