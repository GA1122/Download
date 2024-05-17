WebPluginDelegateStub::~WebPluginDelegateStub() {
  in_destructor_ = true;
  child_process_logging::SetActiveURL(page_url_);

  if (channel_->in_send()) {
    MessageLoop::current()->PostNonNestableTask(FROM_HERE,
        new FinishDestructionTask(delegate_, webplugin_));
  } else {
    if (delegate_)
      delegate_->PluginDestroyed();

    delete webplugin_;
  }
}
