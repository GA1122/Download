scoped_ptr<LayerTreeHost> LayerTreeHost::CreateThreaded(
    LayerTreeHostClient* client,
    SharedBitmapManager* manager,
    const LayerTreeSettings& settings,
    scoped_refptr<base::SingleThreadTaskRunner> impl_task_runner) {
  DCHECK(impl_task_runner);
  scoped_ptr<LayerTreeHost> layer_tree_host(
      new LayerTreeHost(client, manager, settings));
  layer_tree_host->InitializeThreaded(impl_task_runner);
  return layer_tree_host.Pass();
}
