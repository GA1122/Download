 void PluginInstance::OnPluginThreadAsyncCall(void (*func)(void *),
                                             void *user_data) {
  if (webplugin_)
    func(user_data);
}
