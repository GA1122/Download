void WebFrameLoaderClient::finishedLoading(DocumentLoader* dl) {
  if (plugin_widget_.get()) {
    plugin_widget_->didFinishLoading();
    plugin_widget_ = NULL;
    sent_initial_response_to_plugin_ = false;
  } else {
    if (has_representation_)
      dl->frameLoader()->setEncoding("", false);
  }
}
