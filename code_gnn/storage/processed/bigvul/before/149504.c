RequestSender::~RequestSender() {
  DCHECK(thread_checker_.CalledOnValidThread());
}
