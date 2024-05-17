  UsbDeviceHandleTest()
      : io_thread_(base::TestIOThread::kAutoStart),
        scoped_task_environment_(
            base::test::ScopedTaskEnvironment::MainThreadType::UI) {}
