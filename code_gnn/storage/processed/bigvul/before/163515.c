void RecordUnsubscribeGCMResult(gcm::GCMClient::Result result) {
  UMA_HISTOGRAM_ENUMERATION("PushMessaging.UnregistrationGCMResult", result,
                            gcm::GCMClient::LAST_RESULT + 1);
}
