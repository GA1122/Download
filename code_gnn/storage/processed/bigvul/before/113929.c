 void InvalidationNotifier::SetUniqueId(const std::string& unique_id) {
  DCHECK(CalledOnValidThread());
  invalidation_client_id_ = unique_id;
  DVLOG(1) << "Setting unique ID to " << unique_id;
  CHECK(!invalidation_client_id_.empty());
}
