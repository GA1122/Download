  MockCanceledAfterSentPluginProcessHostClient(
      ResourceContext* context)
      : MockCanceledBeforeSentPluginProcessHostClient(context),
        on_sent_plugin_channel_request_called_(false) {}
