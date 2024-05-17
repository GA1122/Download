void WebPluginImpl::updateFocus(bool focused) {
  if (focused && accepts_input_events_)
    delegate_->SetFocus();
}
