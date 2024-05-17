void CloudPolicyCacheBase::AddObserver(Observer* observer) {
  observer_list_.AddObserver(observer);
}
