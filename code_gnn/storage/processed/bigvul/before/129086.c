bool DebuggerGetTargetsFunction::RunAsync() {
  DevToolsTargetImpl::EnumerateAllTargets(
      base::Bind(&DebuggerGetTargetsFunction::SendTargetList, this));
  return true;
}
