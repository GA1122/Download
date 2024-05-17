  void DropSpareRenderProcessHost(RenderProcessHost* host) {
    if (spare_render_process_host_ && spare_render_process_host_ == host) {
      spare_render_process_host_->RemoveObserver(this);
      spare_render_process_host_ = nullptr;
    }
  }
