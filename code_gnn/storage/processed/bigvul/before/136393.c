void PaintArtifactCompositor::LayerizeGroup(
    const PaintArtifact& paint_artifact,
    Vector<PendingLayer>& pending_layers,
    const EffectPaintPropertyNode& current_group,
    Vector<PaintChunk>::const_iterator& chunk_it) {
  if (SkipGroupIfEffectivelyInvisible(paint_artifact, current_group, chunk_it))
    return;

  size_t first_layer_in_current_group = pending_layers.size();
  while (chunk_it != paint_artifact.PaintChunks().end()) {
    const auto* chunk_effect = chunk_it->properties.Effect();
    if (chunk_effect == &current_group) {
      const auto& last_display_item =
          paint_artifact.GetDisplayItemList()[chunk_it->begin_index];
      bool requires_own_layer = last_display_item.IsForeignLayer() ||
                                last_display_item.IsScrollHitTest();
      pending_layers.push_back(PendingLayer(
          *chunk_it, chunk_it - paint_artifact.PaintChunks().begin(),
          requires_own_layer));
      chunk_it++;
      if (requires_own_layer)
        continue;
    } else {
      const EffectPaintPropertyNode* subgroup =
          StrictChildOfAlongPath(&current_group, chunk_effect);
      if (!subgroup)
        break;
      size_t first_layer_in_subgroup = pending_layers.size();
      LayerizeGroup(paint_artifact, pending_layers, *subgroup, chunk_it);
      if (pending_layers.size() != first_layer_in_subgroup + 1)
        continue;
      PendingLayer& subgroup_layer = pending_layers[first_layer_in_subgroup];
      if (!CanDecompositeEffect(subgroup, subgroup_layer))
        continue;
      subgroup_layer.Upcast(PropertyTreeState(
          subgroup->LocalTransformSpace(),
          subgroup->OutputClip() ? subgroup->OutputClip()
                                 : subgroup_layer.property_tree_state.Clip(),
          &current_group));
    }
    const PendingLayer& new_layer = pending_layers.back();
    DCHECK(!new_layer.requires_own_layer);
    DCHECK_EQ(&current_group, new_layer.property_tree_state.Effect());
    for (size_t candidate_index = pending_layers.size() - 1;
         candidate_index-- > first_layer_in_current_group;) {
      PendingLayer& candidate_layer = pending_layers[candidate_index];
      if (candidate_layer.CanMerge(new_layer)) {
        candidate_layer.Merge(new_layer);
        pending_layers.pop_back();
        break;
      }
      if (MightOverlap(new_layer, candidate_layer))
        break;
    }
  }
}
