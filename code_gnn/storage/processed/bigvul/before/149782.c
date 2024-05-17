std::unique_ptr<LayerTreeHost> LayerTreeHost::CreateThreaded(
    scoped_refptr<base::SingleThreadTaskRunner> impl_task_runner,
    InitParams* params) {
  DCHECK(params->main_task_runner.get());
  DCHECK(impl_task_runner.get());
  DCHECK(params->settings);
  std::unique_ptr<LayerTreeHost> layer_tree_host(
      new LayerTreeHost(params, CompositorMode::THREADED));
  layer_tree_host->InitializeThreaded(params->main_task_runner,
                                      impl_task_runner);
  return layer_tree_host;
}
