AutomationProvider::~AutomationProvider() {
  STLDeleteContainerPairSecondPointers(port_containers_.begin(),
                                       port_containers_.end());
  port_containers_.clear();

  if (channel_.get())
    channel_->Close();

  g_browser_process->ReleaseModule();
}
