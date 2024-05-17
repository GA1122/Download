void LayerTreeHostImpl::ElementIsAnimatingChanged(
    ElementId element_id,
    ElementListType list_type,
    const PropertyAnimationState& mask,
    const PropertyAnimationState& state) {
  LayerTreeImpl* tree =
      list_type == ElementListType::ACTIVE ? active_tree() : pending_tree();
  if (!tree)
    return;
  LayerImpl* layer = tree->LayerByElementId(element_id);
  if (layer)
    layer->OnIsAnimatingChanged(mask, state);
}
