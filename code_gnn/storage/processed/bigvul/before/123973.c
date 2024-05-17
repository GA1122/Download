void RenderViewImpl::didFailLoad(WebFrame* frame, const WebURLError& error) {
  WebDataSource* ds = frame->dataSource();
  DCHECK(ds);


  FOR_EACH_OBSERVER(RenderViewObserver, observers_, DidFailLoad(frame, error));

  const WebURLRequest& failed_request = ds->request();
  string16 error_description;
  GetContentClient()->renderer()->GetNavigationErrorStrings(
      failed_request,
      error,
      NULL,
      &error_description);
  Send(new ViewHostMsg_DidFailLoadWithError(routing_id_,
                                            frame->identifier(),
                                            failed_request.url(),
                                            !frame->parent(),
                                            error.reason,
                                            error_description));
}
