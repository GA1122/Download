scoped_ptr<LayerTreeHost> LayerTreeHost::CreateSingleThreaded(
    LayerTreeHostClient* client,
    LayerTreeHostSingleThreadClient* single_thread_client,
    SharedBitmapManager* manager,
    const LayerTreeSettings& settings) {
  scoped_ptr<LayerTreeHost> layer_tree_host(
      new LayerTreeHost(client, manager, settings));
  layer_tree_host->InitializeSingleThreaded(single_thread_client);
  return layer_tree_host.Pass();
}
