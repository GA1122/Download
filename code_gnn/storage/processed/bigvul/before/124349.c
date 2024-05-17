bool MessageService::OpenChannelImpl(scoped_ptr<OpenChannelParams> params) {
  if (!params->source)
    return false;   

  if (!params->receiver.get() || !params->receiver->GetRenderProcessHost()) {
    ExtensionMessagePort port(params->source, MSG_ROUTING_CONTROL, "");
    port.DispatchOnDisconnect(GET_OPPOSITE_PORT_ID(params->receiver_port_id),
                              true);
    return false;
  }

  CHECK(params->receiver->GetRenderProcessHost());

  MessageChannel* channel(new MessageChannel);
  channel->opener.reset(new ExtensionMessagePort(params->source,
                                                 MSG_ROUTING_CONTROL,
                                                 params->source_extension_id));
  channel->receiver.reset(params->receiver.release());

  CHECK(channel->receiver->GetRenderProcessHost());

  AddChannel(channel, params->receiver_port_id);

  CHECK(channel->receiver->GetRenderProcessHost());

  channel->receiver->DispatchOnConnect(params->receiver_port_id,
                                       params->channel_name, params->tab_json,
                                       params->source_extension_id,
                                       params->target_extension_id);

  channel->opener->IncrementLazyKeepaliveCount();
  channel->receiver->IncrementLazyKeepaliveCount();
  return true;
}
