void LayerTreeHost::ReduceMemoryUsage() {
  if (!root_layer())
    return;

  LayerTreeHostCommon::CallFunctionForSubtree(
      root_layer(),
      base::Bind(&LayerTreeHostReduceMemoryCallback));
}
