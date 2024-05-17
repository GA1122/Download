Response InputHandler::SetIgnoreInputEvents(bool ignore) {
  ignore_input_events_ = ignore;
  if (host_)
    host_->GetRenderWidgetHost()->SetIgnoreInputEvents(ignore);
  return Response::OK();
}
