bool MockLogMessageHandler(int, const char*, int, size_t,
                           const std::string& str) {
  if (!g_log_buffer)
    g_log_buffer = new std::string();
  g_log_buffer->append(str);
  return false;
}
