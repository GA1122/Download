RenderProcessHostWatcher::~RenderProcessHostWatcher() {
  if (render_process_host_)
    render_process_host_->RemoveObserver(this);
}
