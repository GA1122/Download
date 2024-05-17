  bool Start(scoped_refptr<base::MessageLoopProxy> message_loop_proxy,
             const tracked_objects::Location& from_here) {
    return message_loop_proxy->PostTask(
        from_here,
        NewRunnableMethod(this, &MessageLoopRelay::ProcessOnTargetThread));
  }
