bool RenderFrameHostImpl::Send(IPC::Message* message) {
  if (IPC_MESSAGE_ID_CLASS(message->type()) == InputMsgStart) {
    return GetRenderWidgetHost()->input_router()->SendInput(
        base::WrapUnique(message));
  }

  return GetProcess()->Send(message);
}
