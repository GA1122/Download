SyncBackendHost::Core::Core(const std::string& name,
                            const FilePath& sync_data_folder_path,
                            const base::WeakPtr<SyncBackendHost>& backend)
    : name_(name),
      sync_data_folder_path_(sync_data_folder_path),
      host_(backend),
      sync_loop_(NULL),
      registrar_(NULL) {
  DCHECK(backend.get());
}
