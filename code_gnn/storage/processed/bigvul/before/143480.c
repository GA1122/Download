  static void ConnectVizFrameSinkManagerOnIOThread(
      viz::mojom::FrameSinkManagerRequest request,
      viz::mojom::FrameSinkManagerClientPtrInfo client) {
    auto* gpu_process_host = GpuProcessHost::Get();
    if (!gpu_process_host)
      return;
    gpu_process_host->gpu_host()->ConnectFrameSinkManager(std::move(request),
                                                          std::move(client));
  }
