  MockCanceledBeforeSentPluginProcessHostClient(
      ResourceContext* context)
      : MockCanceledPluginServiceClient(context),
        set_plugin_info_called_(false),
        on_found_plugin_process_host_called_(false),
        host_(NULL) {}
