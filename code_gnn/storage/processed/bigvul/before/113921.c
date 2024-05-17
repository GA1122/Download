ChromeInvalidationClient::~ChromeInvalidationClient() {
  DCHECK(CalledOnValidThread());
  push_client_->RemoveObserver(this);
  Stop();
  DCHECK(!listener_);
 }
