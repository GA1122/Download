bool RenderWidgetHostImpl::Send(IPC::Message* msg) {
  DCHECK(IPC_MESSAGE_ID_CLASS(msg->type()) != InputMsgStart);
  return process_->Send(msg);
}
