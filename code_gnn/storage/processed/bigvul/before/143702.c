bool RenderWidgetHostImpl::Send(IPC::Message* msg) {
  if (IPC_MESSAGE_ID_CLASS(msg->type()) == InputMsgStart)
    return input_router_->SendInput(make_scoped_ptr(msg));

  return process_->Send(msg);
}
