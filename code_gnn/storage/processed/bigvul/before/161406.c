void DidFindRegistrationForDispatchSyncEventOnIO(
    scoped_refptr<BackgroundSyncContext> sync_context,
    const std::string& tag,
    bool last_chance,
    ServiceWorkerStatusCode status,
    scoped_refptr<content::ServiceWorkerRegistration> registration) {
  if (status != SERVICE_WORKER_OK || !registration->active_version())
    return;
  BackgroundSyncManager* background_sync_manager =
      sync_context->background_sync_manager();
  scoped_refptr<content::ServiceWorkerVersion> version(
      registration->active_version());
  background_sync_manager->EmulateDispatchSyncEvent(
      tag, std::move(version), last_chance,
      base::BindOnce(&StatusNoOpKeepingRegistration, std::move(registration)));
}
