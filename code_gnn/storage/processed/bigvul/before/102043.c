SyncBackendHost::Core::DoInitializeOptions::DoInitializeOptions(
    SyncBackendRegistrar* registrar,
    const WeakHandle<JsEventHandler>& event_handler,
    const GURL& service_url,
    const scoped_refptr<net::URLRequestContextGetter>&
        request_context_getter,
    const sync_api::SyncCredentials& credentials,
    bool delete_sync_data_folder,
    const std::string& restored_key_for_bootstrapping,
    bool setup_for_test_mode)
    : registrar(registrar),
      event_handler(event_handler),
      service_url(service_url),
      request_context_getter(request_context_getter),
      credentials(credentials),
      delete_sync_data_folder(delete_sync_data_folder),
      restored_key_for_bootstrapping(restored_key_for_bootstrapping),
      setup_for_test_mode(setup_for_test_mode) {
}
