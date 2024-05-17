void RenderViewImpl::reportFindInPageMatchCount(int request_id, int count,
                                                bool final_update) {
  int active_match_ordinal = -1;   
  if (!count)
    active_match_ordinal = 0;

  IPC::Message* msg = new ViewHostMsg_Find_Reply(
      routing_id_,
      request_id,
      count,
      gfx::Rect(),
      active_match_ordinal,
      final_update);

  if (queued_find_reply_message_.get()) {
    queued_find_reply_message_.reset(msg);
  } else {
    Send(msg);
  }
}
