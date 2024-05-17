void AppCacheDispatcherHost::StartUpdateCallback(bool result, void* param) {
  IPC::Message* reply_msg = reinterpret_cast<IPC::Message*>(param);
  DCHECK_EQ(pending_reply_msg_.get(), reply_msg);
  AppCacheHostMsg_StartUpdate::WriteReplyParams(reply_msg, result);
  Send(pending_reply_msg_.release());
}
