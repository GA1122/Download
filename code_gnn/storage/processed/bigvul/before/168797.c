Visibility WebContentsImpl::GetVisibility() const {
  if (!should_normally_be_visible_)
    return Visibility::HIDDEN;
  if (should_normally_be_occluded_)
    return Visibility::OCCLUDED;
  return Visibility::VISIBLE;
}
