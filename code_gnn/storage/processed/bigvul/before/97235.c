void WebFrameLoaderClient::setMainDocumentError(DocumentLoader*,
                                                const ResourceError& error) {
  if (plugin_widget_.get()) {
    if (sent_initial_response_to_plugin_) {
      plugin_widget_->didFailLoading(error);
      sent_initial_response_to_plugin_ = false;
    }
    plugin_widget_ = NULL;
  }
}
