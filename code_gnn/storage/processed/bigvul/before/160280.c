void MimeHandlerViewContainer::PluginDidFinishLoading() {
  DCHECK(!is_embedded_);
  CreateMimeHandlerViewGuestIfNecessary();
}
