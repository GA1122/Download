void GLES2DecoderImpl::LogMessage(const std::string& msg) {
  if (log_message_count_ < kMaxLogMessages ||
      CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableGLErrorLimit)) {
    ++log_message_count_;
    if (log_synthesized_gl_errors()) {
      LOG(ERROR) << msg;
    }
    if (!msg_callback_.is_null()) {
      msg_callback_.Run(0, msg);
    }
  } else {
    if (log_message_count_ == kMaxLogMessages) {
      ++log_message_count_;
      LOG(ERROR)
          << "Too many GL errors, not reporting any more for this context."
          << " use --disable-gl-error-limit to see all errors.";
    }
  }
}
