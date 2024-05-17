void RenderViewImpl::didCompleteClientRedirect(
    WebFrame* frame, const WebURL& from) {
  if (!frame->parent()) {
    WebDataSource* ds = frame->provisionalDataSource();
    completed_client_redirect_src_ = Referrer(
        from, ds ? GetReferrerPolicyFromRequest(ds->request()) :
        frame->referrerPolicy());
  }
  FOR_EACH_OBSERVER(
      RenderViewObserver, observers_, DidCompleteClientRedirect(frame, from));
}
