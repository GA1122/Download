bool BackgroundLoaderOffliner::Cancel(CancelCallback callback) {
  DCHECK(pending_request_);
  if (!pending_request_)
    return false;

  if (save_state_ != NONE) {
    save_state_ = DELETE_AFTER_SAVE;
    cancel_callback_ = std::move(callback);
    return true;
  }

  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(std::move(callback), *pending_request_.get()));
  ResetState();
  return true;
}
