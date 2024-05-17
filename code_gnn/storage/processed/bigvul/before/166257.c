void VideoCaptureManager::EmitLogMessage(const std::string& message,
                                         int verbose_log_level) {
  DVLOG(verbose_log_level) << message;
  emit_log_message_cb_.Run(message);
}
