void TabStrip::SetResetToShrinkOnExit(bool value) {
  if (!GetAdjustLayout())
    return;

  if (value && layout_type_ == TAB_STRIP_LAYOUT_SHRINK)
    value = false;   

  if (value == reset_to_shrink_on_exit_)
    return;

  reset_to_shrink_on_exit_ = value;
  if (reset_to_shrink_on_exit_)
    AddMessageLoopObserver();
  else
    RemoveMessageLoopObserver();
}
