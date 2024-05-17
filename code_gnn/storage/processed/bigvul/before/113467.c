void InputMethodIBus::UpdateContextFocusState() {
  if (!context_) {
    context_focused_ = false;
    return;
  }

  const bool old_context_focused = context_focused_;
  switch (GetTextInputType()) {
    case TEXT_INPUT_TYPE_NONE:
    case TEXT_INPUT_TYPE_PASSWORD:
      context_focused_ = false;
      break;
    default:
      context_focused_ = true;
      break;
  }

  if (old_context_focused && !context_focused_)
    ibus_client_->FocusOut(context_);
  else if (!old_context_focused && context_focused_)
    ibus_client_->FocusIn(context_);

  if (context_focused_) {
    internal::IBusClient::InlineCompositionCapability capability =
        CanComposeInline() ? internal::IBusClient::INLINE_COMPOSITION
                           : internal::IBusClient::OFF_THE_SPOT_COMPOSITION;
    ibus_client_->SetCapabilities(context_, capability);
  }
}
