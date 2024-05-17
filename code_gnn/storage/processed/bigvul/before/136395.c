void PaintArtifactCompositor::PendingLayer::Merge(const PendingLayer& guest) {
  DCHECK(!requires_own_layer && !guest.requires_own_layer);

  paint_chunk_indices.AppendVector(guest.paint_chunk_indices);
  FloatClipRect guest_bounds_in_home(guest.bounds);
  GeometryMapper::LocalToAncestorVisualRect(
      guest.property_tree_state, property_tree_state, guest_bounds_in_home);
  bounds.Unite(guest_bounds_in_home.Rect());
}
