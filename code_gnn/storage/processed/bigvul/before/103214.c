void Browser::ToggleEncodingAutoDetect() {
  UserMetrics::RecordAction(UserMetricsAction("AutoDetectChange"), profile_);
  encoding_auto_detect_.SetValue(!encoding_auto_detect_.GetValue());
  if (encoding_auto_detect_.GetValue()) {
    TabContents* contents = GetSelectedTabContents();
    if (contents)
      contents->ResetOverrideEncoding();
  }
}
