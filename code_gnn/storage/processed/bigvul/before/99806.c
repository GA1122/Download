void WebPluginDelegateStub::OnDidReceiveManualData(
    const std::vector<char>& buffer) {
  delegate_->DidReceiveManualData(&buffer.front(),
                                  static_cast<int>(buffer.size()));
}
