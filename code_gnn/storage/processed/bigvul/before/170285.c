WebContentsLoadedOrDestroyedWatcher::WebContentsLoadedOrDestroyedWatcher(
    content::WebContents* web_contents)
    : content::WebContentsObserver(web_contents),
      message_loop_runner_(new content::MessageLoopRunner) {
}
