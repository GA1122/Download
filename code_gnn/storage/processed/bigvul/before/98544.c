  WindowDragResponseTask(AutomationProvider* provider,
                         IPC::Message* reply_message)
      : provider_(provider),
        reply_message_(reply_message) {
    DCHECK(provider_);
    DCHECK(reply_message_);
  }
