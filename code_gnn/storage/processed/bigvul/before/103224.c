void Browser::WorkerCrashed() {
  TabContents* tab_contents = GetSelectedTabContents();
  if (!tab_contents)
    return;
  tab_contents->AddInfoBar(new SimpleAlertInfoBarDelegate(tab_contents, NULL,
      l10n_util::GetStringUTF16(IDS_WEBWORKER_CRASHED_PROMPT), true));
}
