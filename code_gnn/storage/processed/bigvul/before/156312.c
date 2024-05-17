void MediaRecorderHandler::WriteData(base::StringPiece data) {
  DCHECK(main_render_thread_checker_.CalledOnValidThread());
  const TimeTicks now = TimeTicks::Now();
  if (timeslice_.is_zero()) {
    client_->WriteData(data.data(), data.length(), true  ,
                       (now - TimeTicks::UnixEpoch()).InMillisecondsF());
    return;
  }

  const bool last_in_slice = now > slice_origin_timestamp_ + timeslice_;
  DVLOG_IF(1, last_in_slice) << "Slice finished @ " << now;
  if (last_in_slice)
    slice_origin_timestamp_ = now;
  client_->WriteData(data.data(), data.length(), last_in_slice,
                     (now - TimeTicks::UnixEpoch()).InMillisecondsF());
}
