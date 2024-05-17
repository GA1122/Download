void LayerTreeHostImpl::SetElementOpacityMutated(ElementId element_id,
                                                 ElementListType list_type,
                                                 float opacity) {
  if (list_type == ElementListType::ACTIVE) {
    SetTreeLayerOpacityMutated(element_id, active_tree(), opacity);
  } else {
    SetTreeLayerOpacityMutated(element_id, pending_tree(), opacity);
    SetTreeLayerOpacityMutated(element_id, recycle_tree(), opacity);
  }
}
