OfflinePageModelImpl::OfflinePageModelImpl(
    std::unique_ptr<OfflinePageMetadataStore> store,
    std::unique_ptr<ArchiveManager> archive_manager,
    const scoped_refptr<base::SequencedTaskRunner>& task_runner)
    : store_(std::move(store)),
      is_loaded_(false),
      policy_controller_(new ClientPolicyController()),
      archive_manager_(std::move(archive_manager)),
      testing_clock_(nullptr),
      skip_clearing_original_url_for_testing_(false),
      weak_ptr_factory_(this) {
  archive_manager_->EnsureArchivesDirCreated(
      base::Bind(&OfflinePageModelImpl::OnEnsureArchivesDirCreatedDone,
                 weak_ptr_factory_.GetWeakPtr(), base::TimeTicks::Now()));
}
