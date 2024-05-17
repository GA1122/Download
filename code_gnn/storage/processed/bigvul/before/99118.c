ResourceDispatcherHost::ResourceDispatcherHost()
    : ALLOW_THIS_IN_INITIALIZER_LIST(
          download_file_manager_(new DownloadFileManager(this))),
      download_request_manager_(new DownloadRequestManager()),
      ALLOW_THIS_IN_INITIALIZER_LIST(
          save_file_manager_(new SaveFileManager(this))),
      user_script_listener_(new UserScriptListener(&resource_queue_)),
      safe_browsing_(new SafeBrowsingService),
      socket_stream_dispatcher_host_(new SocketStreamDispatcherHost),
      webkit_thread_(new WebKitThread),
      request_id_(-1),
      ALLOW_THIS_IN_INITIALIZER_LIST(method_runner_(this)),
      is_shutdown_(false),
      max_outstanding_requests_cost_per_process_(
          kMaxOutstandingRequestsCostPerProcess),
      receiver_(NULL) {
  ResourceQueue::DelegateSet resource_queue_delegates;
  resource_queue_delegates.insert(user_script_listener_.get());
  resource_queue_.Initialize(resource_queue_delegates);
}
