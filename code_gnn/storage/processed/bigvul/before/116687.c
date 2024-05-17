void RenderViewImpl::dispatchIntent(
    WebFrame* frame, const WebIntentRequest& intentRequest) {
  webkit_glue::WebIntentData intent_data(intentRequest.intent());
  int id = intents_host_->RegisterWebIntent(intentRequest);
  Send(new IntentsHostMsg_WebIntentDispatch(
      routing_id_, intent_data, id));
}
