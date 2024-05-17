bool RenderFrameImpl::IsFTPDirectoryListing() {
  return frame_->GetDocumentLoader()->IsListingFtpDirectory();
}
