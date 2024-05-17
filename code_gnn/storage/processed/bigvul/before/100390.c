  explicit RendererMainThread(const std::string& channel_id)
      : base::Thread("Chrome_InProcRendererThread"),
        channel_id_(channel_id),
        render_process_(NULL) {
  }
