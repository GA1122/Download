  explicit DataUrlCommitObserver(WebContents* web_contents)
      : WebContentsObserver(web_contents),
        message_loop_runner_(new MessageLoopRunner) {}
