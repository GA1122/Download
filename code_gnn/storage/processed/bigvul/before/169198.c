ContextMenuFilter::ContextMenuFilter()
    : content::BrowserMessageFilter(FrameMsgStart),
      message_loop_runner_(new content::MessageLoopRunner),
      handled_(false) {}
