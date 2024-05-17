  explicit NavigationCompletedObserver(content::WebContents* web_contents)
      : content::WebContentsObserver(web_contents),
        message_loop_runner_(new content::MessageLoopRunner) {
    web_contents->ForEachFrame(
        base::BindRepeating(&AddFrameToSet, base::Unretained(&frames_)));
  }
