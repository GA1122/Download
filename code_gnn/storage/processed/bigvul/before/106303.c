SyncBackendHost::Core::DoInitializeOptions::DoInitializeOptions(
    const GURL& service_url,
    sync_api::HttpPostProviderFactory* http_bridge_factory,
    const sync_api::SyncCredentials& credentials,
    bool delete_sync_data_folder,
    const std::string& restored_key_for_bootstrapping,
    bool setup_for_test_mode)
    : service_url(service_url),
      http_bridge_factory(http_bridge_factory),
      credentials(credentials),
      delete_sync_data_folder(delete_sync_data_folder),
      restored_key_for_bootstrapping(restored_key_for_bootstrapping),
      setup_for_test_mode(setup_for_test_mode) {
}
