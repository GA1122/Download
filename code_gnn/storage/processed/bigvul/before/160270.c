void MimeHandlerViewContainer::DidFinishLoading(double  ) {
  DCHECK(is_embedded_);
  CreateMimeHandlerViewGuestIfNecessary();
}
