ChromeInvalidationClient::ChromeInvalidationClient(
    scoped_ptr<notifier::PushClient> push_client)
    : push_client_(push_client.get()),
      chrome_system_resources_(push_client.Pass(),
                               ALLOW_THIS_IN_INITIALIZER_LIST(this)),
      listener_(NULL),
      ticl_state_(DEFAULT_NOTIFICATION_ERROR),
      push_client_state_(DEFAULT_NOTIFICATION_ERROR) {
  DCHECK(CalledOnValidThread());
  push_client_->AddObserver(this);
}
