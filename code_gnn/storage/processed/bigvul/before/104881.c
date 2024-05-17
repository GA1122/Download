void PPB_URLLoader_Impl::Close() {
  if (loader_.get()) {
    loader_->cancel();
  } else if (main_document_loader_) {
    WebFrame* frame = instance()->container()->element().document().frame();
    frame->stopLoading();
  }
}
