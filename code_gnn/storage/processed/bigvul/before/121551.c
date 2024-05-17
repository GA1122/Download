NetworkReaderProxy::~NetworkReaderProxy() {
  if (!job_canceller_.is_null()) {
    job_canceller_.Run();
  }
}
