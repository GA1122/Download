void WebLocalFrameImpl::DispatchUnloadEvent() {
  if (!GetFrame())
    return;
  SubframeLoadingDisabler disabler(GetFrame()->GetDocument());
  GetFrame()->Loader().DispatchUnloadEvent();
}
