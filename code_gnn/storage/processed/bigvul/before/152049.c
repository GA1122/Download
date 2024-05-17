void RenderFrameHostImpl::OnSuddenTerminationDisablerChanged(
    bool present,
    blink::WebSuddenTerminationDisablerType disabler_type) {
  DCHECK_NE(GetSuddenTerminationDisablerState(disabler_type), present);
  if (present) {
    sudden_termination_disabler_types_enabled_ |= disabler_type;
  } else {
    sudden_termination_disabler_types_enabled_ &= ~disabler_type;
  }
}
