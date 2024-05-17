void WebResourceService::OnUnpackError(const std::string& error_message) {
  LOG(ERROR) << error_message;
  EndFetch();
}
