void PluginChannel::OnDestroyInstance(int instance_id,
                                      IPC::Message* reply_msg) {
  for (size_t i = 0; i < plugin_stubs_.size(); ++i) {
    if (plugin_stubs_[i]->instance_id() == instance_id) {
      scoped_refptr<MessageFilter> filter(filter_);
      gfx::NativeViewId window =
          plugin_stubs_[i]->webplugin()->containing_window();
      plugin_stubs_.erase(plugin_stubs_.begin() + i);
      Send(reply_msg);
      RemoveRoute(instance_id);
      MessageLoop::current()->PostNonNestableTask(FROM_HERE, base::Bind(
          &MessageFilter::ReleaseModalDialogEvent, filter.get(), window));
      return;
    }
  }

  NOTREACHED() << "Couldn't find WebPluginDelegateStub to destroy";
}
