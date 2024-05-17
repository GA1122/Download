void WebContentsImpl::UpdateWebContentsVisibility(Visibility visibility) {
  const bool occlusion_is_disabled =
      !base::FeatureList::IsEnabled(features::kWebContentsOcclusion) ||
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableBackgroundingOccludedWindowsForTesting);
  if (occlusion_is_disabled && visibility == Visibility::OCCLUDED)
    visibility = Visibility::VISIBLE;

  if (!did_first_set_visible_) {
    if (visibility == Visibility::VISIBLE) {
      WasShown();
      did_first_set_visible_ = true;
    }


    return;
  }

  if (visibility == visibility_)
    return;

  if (visibility == Visibility::VISIBLE)
    WasShown();
  else if (visibility == Visibility::OCCLUDED)
    WasOccluded();
  else
    WasHidden();
}
