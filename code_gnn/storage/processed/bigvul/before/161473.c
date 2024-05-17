Response TargetHandler::Disable() {
  SetAutoAttach(false, false);
  SetDiscoverTargets(false);
  auto_attached_sessions_.clear();
  attached_sessions_.clear();
  return Response::OK();
}
