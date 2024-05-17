  virtual void SetUp() {
    CommandLine::ForCurrentProcess()->AppendSwitch(
        switches::kEnablePrintPreview);

    RenderViewTest::SetUp();
  }
