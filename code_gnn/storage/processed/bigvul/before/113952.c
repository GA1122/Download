void RegistrationManager::UnregisterId(const invalidation::ObjectId& id) {
  DCHECK(CalledOnValidThread());
  invalidation_client_->Unregister(id);
  RegistrationStatusMap::iterator it = registration_statuses_.find(id);
  if (it == registration_statuses_.end()) {
    DLOG(FATAL) << "UnregisterId called on " << ObjectIdToString(id)
                << " which is not in the registration map";
    return;
  }
  delete it->second;
  registration_statuses_.erase(it);
}
