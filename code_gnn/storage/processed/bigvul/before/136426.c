  PaintArtifactCompositorTest()
      : ScopedSlimmingPaintV2ForTest(true),
        task_runner_(new base::TestSimpleTaskRunner),
        task_runner_handle_(task_runner_) {}
