void RenderViewImpl::dispatchIntent(
    WebFrame* frame, const WebIntentRequest& intentRequest) {
  webkit_glue::WebIntentData intent_data(intentRequest.intent());

  WebKit::WebMessagePortChannelArray* channels =
      intentRequest.intent().messagePortChannelsRelease();
  if (channels) {
    for (size_t i = 0; i < channels->size(); ++i) {
      WebMessagePortChannelImpl* webchannel =
          static_cast<WebMessagePortChannelImpl*>((*channels)[i]);
      intent_data.message_port_ids.push_back(webchannel->message_port_id());
      DCHECK(intent_data.message_port_ids[i] != MSG_ROUTING_NONE);
    }
    delete channels;
  }

  int id = intents_host_->RegisterWebIntent(intentRequest);
  Send(new IntentsHostMsg_WebIntentDispatch(
      routing_id_, intent_data, id));
}
