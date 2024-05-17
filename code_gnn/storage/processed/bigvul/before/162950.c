  void GetManifestAndWait() {
    shell()->web_contents()->GetManifest(
        base::Bind(&ManifestBrowserTest::OnGetManifest,
                   base::Unretained(this)));

    message_loop_runner_ = new MessageLoopRunner();
    message_loop_runner_->Run();
  }
