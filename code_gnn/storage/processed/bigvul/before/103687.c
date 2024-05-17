bool BrowserRenderProcessHost::WaitForUpdateMsg(
    int render_widget_id,
    const base::TimeDelta& max_delay,
    IPC::Message* msg) {
  if (child_process_launcher_.get() && child_process_launcher_->IsStarting())
    return false;

  return widget_helper_->WaitForUpdateMsg(render_widget_id, max_delay, msg);
}
