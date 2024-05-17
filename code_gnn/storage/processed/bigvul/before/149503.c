void RequestSender::SendInternalComplete(int error,
                                         const std::string& response_body,
                                         const std::string& response_etag,
                                         int retry_after_sec) {
  if (!error) {
    if (!use_signing_) {
      base::ThreadTaskRunnerHandle::Get()->PostTask(
          FROM_HERE, base::BindOnce(std::move(request_sender_callback_), 0,
                                    response_body, retry_after_sec));
      return;
    }

    DCHECK(use_signing_);
    DCHECK(signer_);
    if (signer_->ValidateResponse(response_body, response_etag)) {
      base::ThreadTaskRunnerHandle::Get()->PostTask(
          FROM_HERE, base::BindOnce(std::move(request_sender_callback_), 0,
                                    response_body, retry_after_sec));
      return;
    }

    error = static_cast<int>(ProtocolError::RESPONSE_NOT_TRUSTED);
  }

  DCHECK(error);

  if (retry_after_sec <= 0 && ++cur_url_ != urls_.end() &&
      base::ThreadTaskRunnerHandle::Get()->PostTask(
          FROM_HERE, base::BindOnce(&RequestSender::SendInternal,
                                    base::Unretained(this)))) {
    return;
  }

  HandleSendError(error, retry_after_sec);
}
