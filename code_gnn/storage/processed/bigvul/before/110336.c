void Plugin::ReportLoadError(const ErrorInfo& error_info) {
  PLUGIN_PRINTF(("Plugin::ReportLoadError (error='%s')\n",
                 error_info.message().c_str()));
  set_nacl_ready_state(DONE);
  set_nexe_error_reported(true);
  nacl::string message = nacl::string("NaCl module load failed: ") +
      error_info.message();
  set_last_error_string(message);
  AddToConsole(message);
  ShutdownProxy();
  EnqueueProgressEvent(kProgressEventError);
  EnqueueProgressEvent(kProgressEventLoadEnd);

  HistogramEnumerateLoadStatus(error_info.error_code());
}
