CloudPolicySubsystem::ObserverRegistrar::~ObserverRegistrar() {
  if (policy_notifier_)
    policy_notifier_->RemoveObserver(observer_);
}
