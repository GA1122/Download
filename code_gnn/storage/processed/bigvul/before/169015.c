OfflinePageModelTaskified::OfflinePageModelTaskified(
    std::unique_ptr<OfflinePageMetadataStoreSQL> store,
    std::unique_ptr<ArchiveManager> archive_manager,
    const scoped_refptr<base::SequencedTaskRunner>& task_runner,
    std::unique_ptr<base::Clock> clock)
    : store_(std::move(store)),
      archive_manager_(std::move(archive_manager)),
      policy_controller_(new ClientPolicyController()),
      task_queue_(this),
      clock_(std::move(clock)),
      weak_ptr_factory_(this) {
  CreateArchivesDirectoryIfNeeded();
  PostClearLegacyTemporaryPagesTask();
  PostClearCachedPagesTask(true  );
  PostCheckMetadataConsistencyTask(true  );
}
