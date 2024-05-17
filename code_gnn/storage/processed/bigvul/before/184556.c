  GpuChannelHost::GpuChannelHost(
    GpuChannelHostFactory* factory, int gpu_process_id, int client_id)
//     GpuChannelHostFactory* factory, int gpu_host_id, int client_id)
      : factory_(factory),
      gpu_process_id_(gpu_process_id),
        client_id_(client_id),
//       gpu_host_id_(gpu_host_id),
        state_(kUnconnected) {
  }