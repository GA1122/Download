bool LayerTreeHost::UpdateLayers(ResourceUpdateQueue* queue) {
  DCHECK(!output_surface_lost_);

  if (!root_layer())
    return false;

  DCHECK(!root_layer()->parent());

  bool result = UpdateLayers(root_layer(), queue);

  micro_benchmark_controller_.DidUpdateLayers();

  return result || next_commit_forces_redraw_;
}
