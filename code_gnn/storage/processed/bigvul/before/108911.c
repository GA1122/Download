void RenderViewImpl::OnActivateNearestFindResult(int request_id,
                                                 float x, float y) {
  if (!webview())
      return;

  WebFrame* main_frame = webview()->mainFrame();
  WebRect selection_rect;
  int ordinal = main_frame->selectNearestFindMatch(WebFloatPoint(x, y),
                                                   &selection_rect);
  if (ordinal == -1) {
    main_frame->increaseMatchCount(0, request_id);
    return;
  }

  SendFindReply(request_id,
                -1  ,
                ordinal,
                selection_rect,
                true  );
}
