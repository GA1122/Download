MainThreadFrameObserver::~MainThreadFrameObserver() {
  render_widget_host_->GetProcess()->RemoveRoute(routing_id_);
}
