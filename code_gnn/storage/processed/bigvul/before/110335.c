void Plugin::ReportLoadAbort() {
  PLUGIN_PRINTF(("Plugin::ReportLoadAbort\n"));
  set_nacl_ready_state(DONE);
  set_nexe_error_reported(true);
  nacl::string error_string("NaCl module load failed: user aborted");
  set_last_error_string(error_string);
  AddToConsole(error_string);
  ShutdownProxy();
  EnqueueProgressEvent(kProgressEventAbort);
  EnqueueProgressEvent(kProgressEventLoadEnd);

  HistogramEnumerateLoadStatus(ERROR_LOAD_ABORTED);
}
