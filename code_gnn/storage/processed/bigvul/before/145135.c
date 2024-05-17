void GpuProcessHost::ConnectFrameSinkManager(
    viz::mojom::FrameSinkManagerRequest request,
    viz::mojom::FrameSinkManagerClientPtrInfo client) {
  TRACE_EVENT0("gpu", "GpuProcessHost::ConnectFrameSinkManager");
  viz::mojom::FrameSinkManagerParamsPtr params =
      viz::mojom::FrameSinkManagerParams::New();
  params->restart_id = host_id_;
  base::Optional<uint32_t> activation_deadline_in_frames =
      switches::GetDeadlineToSynchronizeSurfaces();
  params->use_activation_deadline = activation_deadline_in_frames.has_value();
  params->activation_deadline_in_frames =
      activation_deadline_in_frames.value_or(0u);
  params->frame_sink_manager = std::move(request);
  params->frame_sink_manager_client = std::move(client);
  gpu_main_ptr_->CreateFrameSinkManager(std::move(params));
}
