void ServiceWorkerContextCore::AddLiveRegistration(
    ServiceWorkerRegistration* registration) {
  DCHECK(!GetLiveRegistration(registration->id()));
  live_registrations_[registration->id()] = registration;
  observer_list_->Notify(
      FROM_HERE, &ServiceWorkerContextCoreObserver::OnNewLiveRegistration,
      registration->id(), registration->scope());
}
