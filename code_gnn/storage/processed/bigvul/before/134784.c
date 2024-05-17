std::string FormatLog(const char* fmt, va_list args) {
  std::string msg = base::StringPrintV(fmt, args);
  if (!msg.empty() && msg[msg.size() - 1] == '\n')
    msg.erase(msg.end() - 1, msg.end());
  return msg;
}
