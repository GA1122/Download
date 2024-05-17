void InitialLoadObserver::ConditionMet() {
  registrar_.RemoveAll();
  if (automation_)
    automation_->OnInitialTabLoadsComplete();
}
