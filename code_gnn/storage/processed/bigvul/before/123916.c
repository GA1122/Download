void RenderViewImpl::OnSynchronousFind(int request_id,
                                       const string16& search_string,
                                       const WebFindOptions& options,
                                       IPC::Message* reply_msg) {
  CHECK(!synchronous_find_reply_message_.get());
  synchronous_find_reply_message_.reset(reply_msg);

  DCHECK(!options.findNext);
  StopFinding(STOP_FIND_ACTION_KEEP_SELECTION);
  synchronous_find_active_match_ordinal_ = -1;

  Find(request_id, search_string, options);
}
