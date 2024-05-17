LayerTreeHost::CreateLayerTreeHostImpl(
    LayerTreeHostImplClient* client) {
  DCHECK(task_runner_provider_->IsImplThread());

  const bool supports_impl_scrolling = task_runner_provider_->HasImplThread();
  std::unique_ptr<MutatorHost> mutator_host_impl =
      mutator_host_->CreateImplInstance(supports_impl_scrolling);

  std::unique_ptr<LayerTreeHostImpl> host_impl = LayerTreeHostImpl::Create(
      settings_, client, task_runner_provider_.get(),
      rendering_stats_instrumentation_.get(), task_graph_runner_,
      std::move(mutator_host_impl), id_, std::move(image_worker_task_runner_));
  host_impl->SetHasGpuRasterizationTrigger(has_gpu_rasterization_trigger_);
  host_impl->SetContentIsSuitableForGpuRasterization(
      content_is_suitable_for_gpu_rasterization_);
  task_graph_runner_ = NULL;
  input_handler_weak_ptr_ = host_impl->AsWeakPtr();
  return host_impl;
}
