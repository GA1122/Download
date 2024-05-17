void Plugin::ReportDeadNexe() {
  PLUGIN_PRINTF(("Plugin::ReportDeadNexe\n"));
  if (ppapi_proxy_ != NULL)
    ppapi_proxy_->ReportDeadNexe();

  if (nacl_ready_state() == DONE && !nexe_error_reported()) {   
    int64_t crash_time = NaClGetTimeOfDayMicroseconds();
    HistogramTimeMedium(
        "NaCl.ModuleUptime.Crash",
        (crash_time - ready_time_) / NACL_MICROS_PER_MILLI);

    nacl::string message = nacl::string("NaCl module crashed");
    set_last_error_string(message);
    AddToConsole(message);

    EnqueueProgressEvent(kProgressEventCrash);
    set_nexe_error_reported(true);
    CHECK(ppapi_proxy_ == NULL || !ppapi_proxy_->is_valid());
    ShutdownProxy();
  }
}
