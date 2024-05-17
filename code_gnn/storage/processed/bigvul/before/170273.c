  void TestScreenshotFile(bool enabled) {
    base::RunLoop run_loop;
    QuitMessageLoopAfterScreenshot observer_(run_loop.QuitClosure());

    ChromeScreenshotGrabber* grabber = ChromeScreenshotGrabber::Get();
    grabber->test_observer_ = &observer_;
    SetScreenshotPolicy(enabled);
    grabber->HandleTakeScreenshotForAllRootWindows();
    run_loop.Run();

    grabber->test_observer_ = nullptr;
  }
