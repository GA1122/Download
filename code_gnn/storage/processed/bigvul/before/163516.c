void RecordUnsubscribeIIDResult(InstanceID::Result result) {
  UMA_HISTOGRAM_ENUMERATION("PushMessaging.UnregistrationIIDResult", result,
                            InstanceID::LAST_RESULT + 1);
}
