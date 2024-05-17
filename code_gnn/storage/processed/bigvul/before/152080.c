bool RenderFrameHostImpl::Send(IPC::Message* message) {
  DCHECK(IPC_MESSAGE_ID_CLASS(message->type()) != InputMsgStart);
  return GetProcess()->Send(message);
}
