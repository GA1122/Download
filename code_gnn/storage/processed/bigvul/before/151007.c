void DefaultBindingsDelegate::InspectedContentsClosing() {
  web_contents_->ClosePage();
}
