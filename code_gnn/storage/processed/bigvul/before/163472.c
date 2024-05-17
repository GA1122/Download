void QuicClientPromisedInfo::Init() {
  cleanup_alarm_.reset(session_->connection()->alarm_factory()->CreateAlarm(
      new QuicClientPromisedInfo::CleanupAlarm(this)));
  cleanup_alarm_->Set(
      session_->connection()->helper()->GetClock()->ApproximateNow() +
      QuicTime::Delta::FromSeconds(kPushPromiseTimeoutSecs));
}
