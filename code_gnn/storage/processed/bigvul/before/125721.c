void RenderViewHostImpl::ToggleSpeechInput() {
  Send(new InputTagSpeechMsg_ToggleSpeechInput(GetRoutingID()));
}
