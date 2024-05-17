bool BrowserRenderProcessHost::WaitForPaintMsg(int render_widget_id,
                                               const base::TimeDelta& max_delay,
                                               IPC::Message* msg) {
  return widget_helper_->WaitForPaintMsg(render_widget_id, max_delay, msg);
}
