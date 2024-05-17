void GLES2DecoderImpl::RenderWarning(
    const char* filename, int line, const std::string& msg) {
  logger_.LogMessage(filename, line, std::string("RENDER WARNING: ") + msg);
}
