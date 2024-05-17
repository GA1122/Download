void AutomationMouseEventProcessor::InvokeCallback(
    const automation::Error& error) {
  if (has_point_)
    completion_callback_.Run(point_);
  else
    error_callback_.Run(error);
  delete this;
}
