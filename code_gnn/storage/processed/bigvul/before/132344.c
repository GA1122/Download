blink::WebMIDIClient* RenderFrameImpl::webMIDIClient() {
  if (!midi_dispatcher_)
    midi_dispatcher_ = new MidiDispatcher(this);
  return midi_dispatcher_;
}
