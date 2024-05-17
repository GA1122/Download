void RenderViewImpl::SendFindReply(int request_id,
                                   int match_count,
                                   int ordinal,
                                   const WebRect& selection_rect,
                                   bool final_status_update) {
#if defined(OS_ANDROID)
  if (synchronous_find_reply_message_.get()) {
    if (final_status_update) {
      ViewMsg_SynchronousFind::WriteReplyParams(
          synchronous_find_reply_message_.get(),
          match_count,
          match_count ? synchronous_find_active_match_ordinal_ : 0);
      Send(synchronous_find_reply_message_.release());
    }
    return;
  }
#endif

  Send(new ViewHostMsg_Find_Reply(routing_id_,
                                  request_id,
                                  match_count,
                                  selection_rect,
                                  ordinal,
                                  final_status_update));
}
