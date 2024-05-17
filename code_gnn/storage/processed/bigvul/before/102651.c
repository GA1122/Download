CloudPolicySubsystem::ObserverRegistrar::ObserverRegistrar(
    CloudPolicySubsystem* cloud_policy_subsystem,
    CloudPolicySubsystem::Observer* observer)
    : observer_(observer) {
  policy_notifier_ = cloud_policy_subsystem->notifier();
  policy_notifier_->AddObserver(observer);
}
