const std::string HttpBridge::GetResponseHeaderValue(
    const std::string& name) const {

  DCHECK_EQ(MessageLoop::current(), created_on_loop_);
  base::AutoLock lock(fetch_state_lock_);
  DCHECK(fetch_state_.request_completed);

  std::string value;
  fetch_state_.response_headers->EnumerateHeader(NULL, name, &value);
  return value;
}
