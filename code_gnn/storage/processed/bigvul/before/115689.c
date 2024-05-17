void HostNPScriptObject::LogDebugInfo(const std::string& message) {
  if (log_debug_info_func_.get()) {
    am_currently_logging_ = true;
    NPVariant log_message;
    STRINGZ_TO_NPVARIANT(message.c_str(), log_message);
    bool is_good = InvokeAndIgnoreResult(log_debug_info_func_.get(),
                                         &log_message, 1);
    if (!is_good) {
      LOG(ERROR) << "ERROR - LogDebugInfo failed\n";
    }
    am_currently_logging_ = false;
  }
}
