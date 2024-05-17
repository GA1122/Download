  static void RequestOnIOThread() {
    auto* gpu_process_host =
        GpuProcessHost::Get(GpuProcessHost::GPU_PROCESS_KIND_SANDBOXED, false);
    if (gpu_process_host) {
      gpu_process_host->RequestHDRStatus(
          base::Bind(&HDRProxy::GotResultOnIOThread));
    } else {
      bool hdr_enabled = false;
      GotResultOnIOThread(hdr_enabled);
    }
  }
