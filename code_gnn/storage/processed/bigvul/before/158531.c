void WebLocalFrameImpl::DispatchUnloadEvent() {
  if (!GetFrame())
    return;
  SubframeLoadingDisabler disabler(GetFrame()->GetDocument());
  IgnoreOpensDuringUnloadCountIncrementer ignore_opens_during_unload(
      GetFrame()->GetDocument());
  GetFrame()->Loader().DispatchUnloadEvent();
}
