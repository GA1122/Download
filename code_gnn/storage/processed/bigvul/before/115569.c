void NetworkChangeNotifierLinux::Thread::ListenForNotifications() {
  char buf[4096];
  int rv = ReadNotificationMessage(buf, arraysize(buf));
  while (rv > 0) {
    if (HandleNetlinkMessage(buf, rv)) {
      VLOG(1) << "Detected IP address changes.";
#if defined(OS_CHROMEOS)
      const int kObserverNotificationDelayMS = 200;
      message_loop()->PostDelayedTask(
          FROM_HERE,
          base::Bind(
              &Thread::NotifyObserversOfIPAddressChange,
              ptr_factory_.GetWeakPtr()),
          kObserverNotificationDelayMS);
#else
      NotifyObserversOfIPAddressChange();
#endif
    }
    rv = ReadNotificationMessage(buf, arraysize(buf));
  }

  if (rv == ERR_IO_PENDING) {
    rv = MessageLoopForIO::current()->WatchFileDescriptor(netlink_fd_, false,
        MessageLoopForIO::WATCH_READ, &netlink_watcher_, this);
    LOG_IF(ERROR, !rv) << "Failed to watch netlink socket: " << netlink_fd_;
  }
}
