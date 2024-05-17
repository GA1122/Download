WebKitTestController::WebKitTestController()
    : main_window_(NULL),
      test_phase_(BETWEEN_TESTS),
      is_leak_detection_enabled_(
          base::CommandLine::ForCurrentProcess()->HasSwitch(
              switches::kEnableLeakDetection)),
      crash_when_leak_found_(false),
      devtools_frontend_(NULL) {
  CHECK(!instance_);
  instance_ = this;

  if (is_leak_detection_enabled_) {
    std::string switchValue =
        base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
            switches::kEnableLeakDetection);
    crash_when_leak_found_ = switchValue == switches::kCrashOnFailure;
  }

  printer_.reset(new WebKitTestResultPrinter(&std::cout, &std::cerr));
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEncodeBinary))
    printer_->set_encode_binary_data(true);
  registrar_.Add(this,
                 NOTIFICATION_RENDERER_PROCESS_CREATED,
                 NotificationService::AllSources());
  GpuDataManager::GetInstance()->AddObserver(this);
  ResetAfterLayoutTest();
}
