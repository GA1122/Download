void BaseSessionService::StartSaveTimer() {
  if (MessageLoop::current() && profile() && !weak_factory_.HasWeakPtrs()) {
    MessageLoop::current()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&BaseSessionService::Save, weak_factory_.GetWeakPtr()),
        base::TimeDelta::FromMilliseconds(kSaveDelayMS));
  }
}
