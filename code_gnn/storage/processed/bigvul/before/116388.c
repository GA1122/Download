bool ChromeContentRendererClient::AllowPopup(const GURL& creator) {
  ChromeV8Context* current_context =
      extension_dispatcher_->v8_context_set().GetCurrent();
  return current_context && !current_context->extension_id().empty();
}
