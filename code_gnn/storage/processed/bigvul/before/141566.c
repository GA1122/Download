void TracingControllerImpl::SetTracingDelegateForTesting(
    std::unique_ptr<TracingDelegate> delegate) {
  if (!delegate) {
    delegate_.reset(GetContentClient()->browser()->GetTracingDelegate());
  } else {
    delegate_ = std::move(delegate);
  }
}
