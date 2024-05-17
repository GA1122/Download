bool WebLocalFrameImpl::DispatchBeforeUnloadEvent(bool is_reload) {
  if (!GetFrame())
    return true;

  return GetFrame()->Loader().ShouldClose(is_reload);
}
