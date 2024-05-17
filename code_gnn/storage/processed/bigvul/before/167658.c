  SetIsInertMessageFilter()
      : content::BrowserMessageFilter(FrameMsgStart),
        message_loop_runner_(new content::MessageLoopRunner),
        msg_received_(false) {}
