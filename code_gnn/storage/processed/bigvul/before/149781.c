LayerTreeHost::CreateSingleThreaded(
    LayerTreeHostSingleThreadClient* single_thread_client,
    InitParams* params) {
  DCHECK(params->settings);
  std::unique_ptr<LayerTreeHost> layer_tree_host(
      new LayerTreeHost(params, CompositorMode::SINGLE_THREADED));
  layer_tree_host->InitializeSingleThreaded(single_thread_client,
                                            params->main_task_runner);
  return layer_tree_host;
}
