HostNPScriptObject::HostNPScriptObject(
    NPP plugin,
    NPObject* parent,
    PluginMessageLoopProxy::Delegate* plugin_thread_delegate)
    : plugin_(plugin),
      parent_(parent),
      state_(kDisconnected),
      np_thread_id_(base::PlatformThread::CurrentId()),
      plugin_message_loop_proxy_(
          new PluginMessageLoopProxy(plugin_thread_delegate)),
      host_context_(plugin_message_loop_proxy_),
      failed_login_attempts_(0),
      disconnected_event_(true, false),
      am_currently_logging_(false),
      nat_traversal_enabled_(false),
      policy_received_(false) {
}
