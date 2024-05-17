void RenderViewImpl::didFinishLoad(WebFrame* frame) {
  WebDataSource* ds = frame->dataSource();
  DocumentState* document_state = DocumentState::FromDataSource(ds);
  if (document_state->finish_load_time().is_null())
    document_state->set_finish_load_time(Time::Now());

  FOR_EACH_OBSERVER(RenderViewObserver, observers_, DidFinishLoad(frame));

  Send(new ViewHostMsg_DidFinishLoad(routing_id_,
                                     frame->identifier(),
                                     ds->request().url(),
                                     !frame->parent()));
}
