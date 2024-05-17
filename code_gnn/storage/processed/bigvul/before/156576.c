SessionStore::Factory SessionStore::CreateFactory(
    SyncSessionsClient* sessions_client,
    const RestoredForeignTabCallback& restored_foreign_tab_callback) {
  auto factory = std::make_unique<FactoryImpl>(sessions_client,
                                               restored_foreign_tab_callback);
  return base::BindRepeating(&FactoryImpl::Create, std::move(factory));
}
