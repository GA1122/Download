NetworkChangeNotifierLinux::~NetworkChangeNotifierLinux() {
  notifier_thread_->Stop();
 }
