  void OnReadAllData(const SessionStore::SessionInfo& session_info,
                     SessionStore::FactoryCompletionCallback callback,
                     std::unique_ptr<ModelTypeStore> store,
                     const base::Optional<syncer::ModelError>& error,
                     std::unique_ptr<ModelTypeStore::RecordList> record_list) {
    if (error) {
      std::move(callback).Run(error,  nullptr,
                               nullptr);
      return;
    }

    store->ReadAllMetadata(base::BindOnce(
        &FactoryImpl::OnReadAllMetadata, base::AsWeakPtr(this), session_info,
        std::move(callback), std::move(store), std::move(record_list)));
  }
