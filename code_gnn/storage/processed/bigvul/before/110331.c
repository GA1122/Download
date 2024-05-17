void Plugin::ProcessNaClManifest(const nacl::string& manifest_json) {
  HistogramSizeKB("NaCl.Perf.Size.Manifest",
                  static_cast<int32_t>(manifest_json.length() / 1024));
  nacl::string program_url;
  nacl::string cache_identity;
  bool is_portable;
  ErrorInfo error_info;
  if (!SetManifestObject(manifest_json, &error_info)) {
    ReportLoadError(error_info);
    return;
  }

  if (manifest_->GetProgramURL(&program_url, &cache_identity,
                               &error_info, &is_portable)) {
    set_nacl_ready_state(LOADING);
    EnqueueProgressEvent(kProgressEventProgress);
    if (is_portable) {
      pp::CompletionCallback translate_callback =
          callback_factory_.NewCallback(&Plugin::BitcodeDidTranslate);
      pnacl_coordinator_.reset(
          PnaclCoordinator::BitcodeToNative(this,
                                            program_url,
                                            cache_identity,
                                            translate_callback));
      return;
    } else {
      pp::CompletionCallback open_callback =
          callback_factory_.NewCallback(&Plugin::NexeFileDidOpen);
      CHECK(
          nexe_downloader_.Open(program_url,
                                DOWNLOAD_TO_FILE,
                                open_callback,
                                &UpdateDownloadProgress));
      return;
    }
  }
  ReportLoadError(error_info);
}
