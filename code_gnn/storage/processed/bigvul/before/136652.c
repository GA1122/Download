static NavigationType DetermineNavigationType(FrameLoadType frame_load_type,
                                              bool is_form_submission,
                                              bool have_event) {
  bool is_reload = IsReloadLoadType(frame_load_type);
  bool is_back_forward = IsBackForwardLoadType(frame_load_type);
  if (is_form_submission) {
    return (is_reload || is_back_forward) ? kNavigationTypeFormResubmitted
                                          : kNavigationTypeFormSubmitted;
  }
  if (have_event)
    return kNavigationTypeLinkClicked;
  if (is_reload)
    return kNavigationTypeReload;
  if (is_back_forward)
    return kNavigationTypeBackForward;
  return kNavigationTypeOther;
}
