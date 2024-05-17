bool FitToPageEnabled(const base::DictionaryValue& job_settings) {
  bool fit_to_paper_size = false;
  if (!job_settings.GetBoolean(kSettingFitToPageEnabled, &fit_to_paper_size)) {
    NOTREACHED();
  }
  return fit_to_paper_size;
}
