  void CleanupSpareRenderProcessHost() {
    if (spare_render_process_host_) {
      spare_render_process_host_->Cleanup();
      DropSpareRenderProcessHost(spare_render_process_host_);
    }
  }
