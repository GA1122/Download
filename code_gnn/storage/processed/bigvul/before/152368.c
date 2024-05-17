ui::PageTransition GetTransitionType(ui::PageTransition default_transition,
                                     bool replaces_current_item,
                                     bool is_main_frame,
                                     WebNavigationType navigation_type) {
  if (replaces_current_item && !is_main_frame) {
    return ui::PAGE_TRANSITION_AUTO_SUBFRAME;
  }
  bool is_form_submit =
      navigation_type == blink::kWebNavigationTypeFormSubmitted ||
      navigation_type == blink::kWebNavigationTypeFormResubmitted;
  if (ui::PageTransitionCoreTypeIs(default_transition,
                                   ui::PAGE_TRANSITION_LINK) &&
      is_form_submit) {
    return ui::PAGE_TRANSITION_FORM_SUBMIT;
  }
  return default_transition;
}
