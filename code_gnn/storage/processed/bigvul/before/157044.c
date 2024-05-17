void MultibufferDataSource::SeekTask_Locked() {
  DCHECK(render_task_runner_->BelongsToCurrentThread());
  lock_.AssertAcquired();

  if (stop_signal_received_)
    return;

  if (read_op_)
    return;

  url_data()->AddBytesRead(bytes_read_);
  bytes_read_ = 0;

  if (reader_) {

    int64_t pos = reader_->Tell();
    int64_t available = reader_->Available();

    for (auto i = seek_positions_.rbegin(); i != seek_positions_.rend(); ++i) {
      int64_t new_pos = *i;
      int64_t available_at_new_pos = reader_->AvailableAt(new_pos);

      if (total_bytes_ != kPositionNotSpecified) {
        if (new_pos + available_at_new_pos >= total_bytes_) {
          continue;
        }
      }
      if (available_at_new_pos < available) {
        pos = new_pos;
        available = available_at_new_pos;
      }
    }
    reader_->Seek(pos);
  }
  seek_positions_.clear();

  UpdateLoadingState_Locked(false);
}
