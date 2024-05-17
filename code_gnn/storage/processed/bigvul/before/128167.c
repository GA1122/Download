  void ConnectToChannel(const IPC::ChannelHandle& handle, bool is_ppapi) {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);

    if (!is_removing_)
      return;

    DCHECK(!channel_.get());
    channel_ = IPC::Channel::CreateClient(handle, this);
    if (!channel_->Connect()) {
      NOTREACHED() << "Couldn't connect to plugin";
      SignalDone();
      return;
    }

    uint64 max_age = begin_time_.is_null() ?
        std::numeric_limits<uint64>::max() :
        (base::Time::Now() - begin_time_).InSeconds();

    IPC::Message* msg;
    if (is_ppapi) {
      msg = CreatePpapiClearSiteDataMsg(max_age);
    } else {
      msg = new PluginProcessMsg_ClearSiteData(
          std::string(), kClearAllData, max_age);
    }
    if (!channel_->Send(msg)) {
      NOTREACHED() << "Couldn't send ClearSiteData message";
      SignalDone();
      return;
    }
  }
