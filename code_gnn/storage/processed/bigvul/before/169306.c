RenderProcessHostWatcher::RenderProcessHostWatcher(WebContents* web_contents,
                                                   WatchType type)
    : render_process_host_(web_contents->GetMainFrame()->GetProcess()),
      type_(type),
      did_exit_normally_(true),
      message_loop_runner_(new MessageLoopRunner) {
  render_process_host_->AddObserver(this);
}
