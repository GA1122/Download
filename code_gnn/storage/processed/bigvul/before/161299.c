Response InspectorHandler::Enable() {
  if (host_ && !host_->IsRenderFrameLive())
    frontend_->TargetCrashed();
  return Response::OK();
}
