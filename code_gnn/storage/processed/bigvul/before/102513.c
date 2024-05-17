  MessageLoopRelay()
      : origin_message_loop_proxy_(
            base::MessageLoopProxy::current()),
        error_code_(base::PLATFORM_FILE_OK) {
  }
