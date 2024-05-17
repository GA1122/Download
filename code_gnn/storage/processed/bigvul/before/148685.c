  SkiaOutputSurfaceImplTest()
      : output_surface_client_(std::make_unique<cc::FakeOutputSurfaceClient>()),
        wait_(base::WaitableEvent::ResetPolicy::AUTOMATIC,
              base::WaitableEvent::InitialState::NOT_SIGNALED),
        on_screen_(GetParam()) {}
