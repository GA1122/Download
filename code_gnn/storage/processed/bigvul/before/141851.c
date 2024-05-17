void PageFormAnalyserLogger::Send(std::string message,
                                  ConsoleLevel level,
                                  std::vector<blink::WebNode> nodes) {
  node_buffer_[level].push_back(LogEntry{std::move(message), std::move(nodes)});
}
