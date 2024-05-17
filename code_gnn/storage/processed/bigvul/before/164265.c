  void SendLogMessage(const std::string& message) {
    for (auto* frontend : hosts_to_notify_)
      frontend->LogMessage(blink::mojom::ConsoleMessageLevel::kWarning,
                           message);
  }
