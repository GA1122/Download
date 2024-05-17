  explicit RendererMainThread(const std::string& channel_id)
      : Thread("Chrome_InProcRendererThread"),
        channel_id_(channel_id) {
  }
