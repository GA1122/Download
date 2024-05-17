PasswordStoreLoginsChangedObserver::~PasswordStoreLoginsChangedObserver() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
}
