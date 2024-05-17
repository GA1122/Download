void Browser::ToggleEncodingAutoDetect() {
  content::RecordAction(UserMetricsAction("AutoDetectChange"));
  encoding_auto_detect_.SetValue(!encoding_auto_detect_.GetValue());
  if (encoding_auto_detect_.GetValue()) {
    WebContents* contents = chrome::GetActiveWebContents(this);
    if (contents)
      contents->ResetOverrideEncoding();
  }
}
