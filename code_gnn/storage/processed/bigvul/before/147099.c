void WebLocalFrameImpl::StopLoading() {
  if (!GetFrame())
    return;
  GetFrame()->Loader().StopAllLoaders();
}
