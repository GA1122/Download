void BlobURLRequestJob::DidCountSize(int error) {
  DCHECK(!error_);

  if (error != net::OK) {
    NotifyFailure(error);
    return;
  }

  if (!byte_range_.ComputeBounds(total_size_)) {
    NotifyFailure(net::ERR_REQUEST_RANGE_NOT_SATISFIABLE);
    return;
  }

  remaining_bytes_ = byte_range_.last_byte_position() -
                     byte_range_.first_byte_position() + 1;
  DCHECK_GE(remaining_bytes_, 0);

  if (byte_range_.first_byte_position())
    Seek(byte_range_.first_byte_position());

  NotifySuccess();
}
