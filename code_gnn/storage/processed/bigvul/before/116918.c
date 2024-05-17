  Browser* Restore() {
    SessionService* session_service =
        SessionServiceFactory::GetForProfile(profile_);
    DCHECK(session_service);
    session_service->GetLastSession(
        &request_consumer_,
        base::Bind(&SessionRestoreImpl::OnGotSession, base::Unretained(this)));

    if (synchronous_) {
      bool old_state = MessageLoop::current()->NestableTasksAllowed();
      MessageLoop::current()->SetNestableTasksAllowed(true);
      MessageLoop::current()->Run();
      MessageLoop::current()->SetNestableTasksAllowed(old_state);
      Browser* browser = ProcessSessionWindows(&windows_);
      delete this;
      return browser;
    }

    if (browser_) {
      registrar_.Add(this, chrome::NOTIFICATION_BROWSER_CLOSED,
                     content::Source<Browser>(browser_));
    }

    return browser_;
  }
