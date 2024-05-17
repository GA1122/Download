  void OnStoreCreated(const SessionStore::SessionInfo& session_info,
                      SessionStore::FactoryCompletionCallback callback,
                      const base::Optional<syncer::ModelError>& error,
                      std::unique_ptr<ModelTypeStore> store) {
    if (error) {
      std::move(callback).Run(error,  nullptr,
                               nullptr);
      return;
    }

    DCHECK(store);
    ModelTypeStore* store_copy = store.get();
    store_copy->ReadAllData(
        base::BindOnce(&FactoryImpl::OnReadAllData, base::AsWeakPtr(this),
                       session_info, std::move(callback), std::move(store)));
  }
