void NetworkReaderProxy::OnCompleted(FileError error) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  job_canceller_.Reset();

  if (error == FILE_ERROR_OK) {
    return;
  }

  error_code_ =
      net::PlatformFileErrorToNetError(FileErrorToPlatformError(error));
  pending_data_.clear();

  if (callback_.is_null()) {
    return;
  }

  buffer_ = NULL;
  buffer_length_ = 0;
  base::ResetAndReturn(&callback_).Run(error_code_);
}
