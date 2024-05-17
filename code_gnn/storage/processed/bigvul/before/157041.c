void MultibufferDataSource::ReadTask() {
  DCHECK(render_task_runner_->BelongsToCurrentThread());

  base::AutoLock auto_lock(lock_);
  int bytes_read = 0;
  if (stop_signal_received_ || !read_op_)
    return;
  DCHECK(read_op_->size());

  if (!reader_)
    CreateResourceLoader_Locked(read_op_->position(), kPositionNotSpecified);

  int64_t available = reader_->AvailableAt(read_op_->position());
  if (available < 0) {
    ReadOperation::Run(std::move(read_op_), kReadError);
    return;
  }
  if (available) {
    bytes_read =
        static_cast<int>(std::min<int64_t>(available, read_op_->size()));
    bytes_read =
        reader_->TryReadAt(read_op_->position(), read_op_->data(), bytes_read);

    bytes_read_ += bytes_read;
    seek_positions_.push_back(read_op_->position() + bytes_read);

    if (bytes_read == 0 && total_bytes_ == kPositionNotSpecified) {
      total_bytes_ = read_op_->position() + bytes_read;
      if (total_bytes_ != kPositionNotSpecified)
        host_->SetTotalBytes(total_bytes_);
    }

    ReadOperation::Run(std::move(read_op_), bytes_read);

    SeekTask_Locked();
  } else {
    reader_->Seek(read_op_->position());
    reader_->Wait(1, base::Bind(&MultibufferDataSource::ReadTask,
                                weak_factory_.GetWeakPtr()));
    UpdateLoadingState_Locked(false);
  }
}
