 void WebContentsImpl::OnDialogClosed(int render_process_id,
                                      int render_frame_id,
                                      IPC::Message* reply_msg,
                                     bool dialog_was_suppressed,
                                     bool success,
                                     const base::string16& user_input) {
  RenderFrameHostImpl* rfh = RenderFrameHostImpl::FromID(render_process_id,
                                                         render_frame_id);
  last_dialog_suppressed_ = dialog_was_suppressed;

  if (is_showing_before_unload_dialog_ && !success) {
    if (rfh)
      DidStopLoading();
    controller_.DiscardNonCommittedEntries();

    FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                      BeforeUnloadDialogCancelled());
  }

  is_showing_before_unload_dialog_ = false;
  if (rfh) {
    rfh->JavaScriptDialogClosed(reply_msg, success, user_input,
                                dialog_was_suppressed);
  } else {
    delete reply_msg;
  }
}
