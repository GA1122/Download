bool FitToPageEnabled(const DictionaryValue& job_settings) {
  bool fit_to_paper_size = false;
  if (!job_settings.GetBoolean(printing::kSettingFitToPageEnabled,
                               &fit_to_paper_size)) {
    NOTREACHED();
  }
  return fit_to_paper_size;
}
