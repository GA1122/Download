bool RenderViewHostImpl::SuddenTerminationAllowed() const {
  return sudden_termination_allowed_ ||
      GetProcess()->SuddenTerminationAllowed();
}
