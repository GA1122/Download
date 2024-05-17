void SyncManager::SyncInternal::OnIPAddressChanged() {
  VLOG(1) << "IP address change detected";
  if (!observing_ip_address_changes_) {
    VLOG(1) << "IP address change dropped.";
    return;
  }

#if defined (OS_CHROMEOS)
  MessageLoop::current()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&SyncInternal::OnIPAddressChangedImpl,
                 weak_ptr_factory_.GetWeakPtr()),
      kChromeOSNetworkChangeReactionDelayHackMsec);
#else
  OnIPAddressChangedImpl();
#endif   
}
