void AppCacheDispatcherHost::GetStatusCallback(
    AppCacheStatus status, void* param) {
  IPC::Message* reply_msg = reinterpret_cast<IPC::Message*>(param);
  DCHECK_EQ(pending_reply_msg_.get(), reply_msg);
  AppCacheHostMsg_GetStatus::WriteReplyParams(reply_msg, status);
  Send(pending_reply_msg_.release());
}
