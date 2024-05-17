void NavigationRequest::SetWaitingForRendererResponse() {
  TRACE_EVENT_ASYNC_STEP_INTO0("navigation", "NavigationRequest", this,
                               "WaitingForRendererResponse");
  DCHECK(state_ == NOT_STARTED);
  state_ = WAITING_FOR_RENDERER_RESPONSE;
}
