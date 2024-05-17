ConsoleObserverDelegate::ConsoleObserverDelegate(WebContents* web_contents,
                                                 const std::string& filter)
    : web_contents_(web_contents),
      filter_(filter),
      message_loop_runner_(new MessageLoopRunner) {}
