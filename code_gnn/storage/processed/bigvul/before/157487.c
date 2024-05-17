void IsPinnedToTaskbarHelper::OnConnectionError() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  error_callback_.Run();
  delete this;
}
