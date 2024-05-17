void GLES2DecoderImpl::PerformanceWarning(
    const char* filename, int line, const std::string& msg) {
  logger_.LogMessage(filename, line,
                     std::string("PERFORMANCE WARNING: ") + msg);
}
