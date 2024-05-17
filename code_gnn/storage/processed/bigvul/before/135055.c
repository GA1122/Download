AppCacheHost::AppCacheHost(int host_id, AppCacheFrontend* frontend,
                           AppCacheServiceImpl* service)
    : host_id_(host_id),
      spawning_host_id_(kAppCacheNoHostId), spawning_process_id_(0),
      parent_host_id_(kAppCacheNoHostId), parent_process_id_(0),
      pending_main_resource_cache_id_(kAppCacheNoCacheId),
      pending_selected_cache_id_(kAppCacheNoCacheId),
      was_select_cache_called_(false),
      is_cache_selection_enabled_(true),
      frontend_(frontend), service_(service),
      storage_(service->storage()),
      pending_callback_param_(NULL),
      main_resource_was_namespace_entry_(false),
      main_resource_blocked_(false),
      associated_cache_info_pending_(false) {
  service_->AddObserver(this);
}
