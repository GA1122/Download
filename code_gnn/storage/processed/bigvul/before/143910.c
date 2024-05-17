bool WebRunnerBrowserContext::IsOffTheRecord() const {
  return data_dir_path_.empty();
}
