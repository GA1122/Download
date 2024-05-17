void FragmentPaintPropertyTreeBuilder::UpdateLocalBorderBoxContext() {
  if (!NeedsPaintPropertyUpdate())
    return;

  if (!object_.HasLayer() && !NeedsPaintOffsetTranslation(object_) &&
      !NeedsFilter(object_)) {
    fragment_data_.ClearLocalBorderBoxProperties();
  } else {
    PropertyTreeState local_border_box =
        PropertyTreeState(context_.current.transform, context_.current.clip,
                          context_.current_effect);

    if (RuntimeEnabledFeatures::SlimmingPaintV175Enabled() &&
        (!fragment_data_.HasLocalBorderBoxProperties() ||
         local_border_box != fragment_data_.LocalBorderBoxProperties()))
      property_added_or_removed_ = true;

    fragment_data_.SetLocalBorderBoxProperties(std::move(local_border_box));
  }
}
