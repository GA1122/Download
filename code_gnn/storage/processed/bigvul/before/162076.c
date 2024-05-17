  void DropSpareOnProcessCreation(RenderProcessHost* new_host) {
    if (spare_render_process_host_ == new_host) {
      DropSpareRenderProcessHost(new_host);
    } else {
      CleanupSpareRenderProcessHost();
    }
  }
