void LayerTreeHostImpl::SetElementTransformMutated(
    ElementId element_id,
    ElementListType list_type,
    const gfx::Transform& transform) {
  if (list_type == ElementListType::ACTIVE) {
    SetTreeLayerTransformMutated(element_id, active_tree(), transform);
  } else {
    SetTreeLayerTransformMutated(element_id, pending_tree(), transform);
    SetTreeLayerTransformMutated(element_id, recycle_tree(), transform);
  }
}
