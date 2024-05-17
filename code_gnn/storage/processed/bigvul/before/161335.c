void NetworkHandler::NavigationPreloadCompleted(
    const std::string& request_id,
    const network::URLLoaderCompletionStatus& status) {
  if (!enabled_)
    return;
  if (status.error_code != net::OK) {
    frontend_->LoadingFailed(
        request_id,
        base::TimeTicks::Now().ToInternalValue() /
            static_cast<double>(base::Time::kMicrosecondsPerSecond),
        Page::ResourceTypeEnum::Other, net::ErrorToString(status.error_code),
        status.error_code == net::Error::ERR_ABORTED);
  }
  frontend_->LoadingFinished(
      request_id,
      status.completion_time.ToInternalValue() /
          static_cast<double>(base::Time::kMicrosecondsPerSecond),
      status.encoded_data_length);
}
