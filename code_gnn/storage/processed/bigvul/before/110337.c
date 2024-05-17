void Plugin::ReportLoadSuccess(LengthComputable length_computable,
                               uint64_t loaded_bytes,
                               uint64_t total_bytes) {
  set_nacl_ready_state(DONE);
  const nacl::string& url = nexe_downloader_.url_to_open();
  EnqueueProgressEvent(
      kProgressEventLoad, url, length_computable, loaded_bytes, total_bytes);
  EnqueueProgressEvent(
      kProgressEventLoadEnd, url, length_computable, loaded_bytes, total_bytes);

  HistogramEnumerateLoadStatus(ERROR_LOAD_SUCCESS);
}
