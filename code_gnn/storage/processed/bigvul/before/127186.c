DaemonProcess::~DaemonProcess() {
  DCHECK(!config_watcher_.get());
  DCHECK(desktop_sessions_.empty());
}
