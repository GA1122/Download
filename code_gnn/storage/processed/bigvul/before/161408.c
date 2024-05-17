void DispatchSyncEventOnIO(scoped_refptr<ServiceWorkerContextWrapper> context,
                           scoped_refptr<BackgroundSyncContext> sync_context,
                           const GURL& origin,
                           int64_t registration_id,
                           const std::string& tag,
                           bool last_chance) {
  context->FindReadyRegistrationForId(
      registration_id, origin,
      base::Bind(&DidFindRegistrationForDispatchSyncEventOnIO, sync_context,
                 tag, last_chance));
}
