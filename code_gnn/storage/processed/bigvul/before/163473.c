void QuicClientPromisedInfo::CleanupAlarm::OnAlarm() {
  QUIC_DVLOG(1) << "self GC alarm for stream " << promised_->id_;
  promised_->session()->OnPushStreamTimedOut(promised_->id_);
  promised_->Reset(QUIC_PUSH_STREAM_TIMED_OUT);
}
