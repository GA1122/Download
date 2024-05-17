void WebContentsImpl::ActivateNearestFindResult(float x,
                                                float y) {
  GetOrCreateFindRequestManager()->ActivateNearestFindResult(x, y);
}
