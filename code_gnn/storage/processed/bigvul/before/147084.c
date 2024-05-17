void WebLocalFrameImpl::SetInputEventsScaleForEmulation(
    float content_scale_factor) {
  input_events_scale_factor_for_emulation_ = content_scale_factor;
  if (GetFrame()->View()) {
    GetFrame()->View()->SetInputEventsScaleForEmulation(
        input_events_scale_factor_for_emulation_);
  }
}
