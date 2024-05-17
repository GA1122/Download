void PushMessagingServiceImpl::DidUnregister(bool was_subscribed,
                                             gcm::GCMClient::Result result) {
  RecordUnsubscribeGCMResult(result);
  DidUnsubscribe(std::string()  , was_subscribed);
}
