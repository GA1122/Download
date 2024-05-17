void SyncManager::TriggerOnIncomingNotificationForTest(
    ModelTypeSet model_types) {
  DCHECK(thread_checker_.CalledOnValidThread());
  syncable::ModelTypePayloadMap model_types_with_payloads =
      syncable::ModelTypePayloadMapFromEnumSet(model_types,
          std::string());

  data_->OnIncomingNotification(model_types_with_payloads,
                                sync_notifier::REMOTE_NOTIFICATION);
}
