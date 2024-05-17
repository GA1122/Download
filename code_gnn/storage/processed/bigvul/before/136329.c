bool PaintPropertyTreeBuilder::UpdateForChildren() {
  if (!ObjectTypeMightNeedPaintProperties())
    return false;

  bool property_changed = false;
  bool property_added_or_removed = false;
  auto* fragment_data = &object_.GetMutableForPainting().FirstFragment();
  for (auto& fragment_context : context_.fragments) {
    FragmentPaintPropertyTreeBuilder builder(object_, context_,
                                             fragment_context, *fragment_data);
    builder.UpdateForChildren();
    property_changed |= builder.PropertyChanged();
    property_added_or_removed |= builder.PropertyAddedOrRemoved();
    context_.force_subtree_update |= object_.SubtreeNeedsPaintPropertyUpdate();
    fragment_data = fragment_data->NextFragment();
  }
  DCHECK(!fragment_data);

  if (object_.CanContainAbsolutePositionObjects())
    context_.container_for_absolute_position = &object_;
  if (object_.CanContainFixedPositionObjects())
    context_.container_for_fixed_position = &object_;

  if (property_added_or_removed &&
      RuntimeEnabledFeatures::SlimmingPaintV175Enabled())
    context_.painting_layer->SetNeedsRepaint();

  return property_changed;
}
