void WebPluginDelegateProxy::OnSetWindowlessPumpEvent(
      HANDLE modal_loop_pump_messages_event) {
  DCHECK(modal_loop_pump_messages_event_ == NULL);

  if (!modal_loop_pump_messages_event)
    return;

  modal_loop_pump_messages_event_.reset(
      new base::WaitableEvent(modal_loop_pump_messages_event));
}
