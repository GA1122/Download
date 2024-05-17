 void ChromotingHost::AddStatusObserver(HostStatusObserver* observer) {
  DCHECK_EQ(state_, kInitial);
  status_observers_.push_back(observer);
}
