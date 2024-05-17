  void OnGetManifest(const GURL& manifest_url, const Manifest& manifest) {
    manifest_url_ = manifest_url;
    manifest_ = manifest;
    message_loop_runner_->Quit();
  }
