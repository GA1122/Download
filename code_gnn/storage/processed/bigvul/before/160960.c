void ChromeClientImpl::FullscreenElementChanged(Element* old_element,
                                                Element* new_element) {
  web_view_->FullscreenElementChanged(old_element, new_element);
}
