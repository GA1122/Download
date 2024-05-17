scoped_refptr<base::debug::ConvertableToTraceFormat> Layer::TakeDebugInfo() {
  if (client_)
    return client_->TakeDebugInfo();
  else
    return NULL;
}
