void WebPluginImpl::didReceiveData(const char* data, int data_length) {
  delegate_->DidReceiveManualData(data, data_length);
}
