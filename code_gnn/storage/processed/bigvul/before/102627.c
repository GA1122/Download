CloudPolicyCacheBase::~CloudPolicyCacheBase() {
  FOR_EACH_OBSERVER(Observer, observer_list_, OnCacheGoingAway(this));
}
