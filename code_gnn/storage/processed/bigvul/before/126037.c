void PasswordStoreLoginsChangedObserver::RegisterObserversTask() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::DB));
  registrar_.reset(new content::NotificationRegistrar);
  registrar_->Add(this, chrome::NOTIFICATION_LOGINS_CHANGED,
                  content::NotificationService::AllSources());
  done_event_.Signal();
}
