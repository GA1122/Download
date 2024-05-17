  DataURLWarningConsoleObserverDelegate(
      WebContents* web_contents,
      ExpectedNavigationStatus expected_navigation_status)
      : web_contents_(web_contents),
        success_filter_(expected_navigation_status == NAVIGATION_ALLOWED
                            ? kDataUrlSuccessfulMessage
                            : kDataUrlBlockedPattern),
        fail_filter_(expected_navigation_status == NAVIGATION_ALLOWED
                         ? kDataUrlBlockedPattern
                         : kDataUrlSuccessfulMessage),
        message_loop_runner_(
            new MessageLoopRunner(MessageLoopRunner::QuitMode::IMMEDIATE)),
        saw_failure_message_(false) {}
