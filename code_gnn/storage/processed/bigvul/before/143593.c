  void RunDetection(bool renderer_pause_enabled, bool navigate_ads_enabled) {
    mojom::blink::OomInterventionHostPtr host_ptr;
    MockOomInterventionHost mock_host(mojo::MakeRequest(&host_ptr));

    mojom::blink::DetectionArgsPtr args(mojom::blink::DetectionArgs::New());
    args->blink_workload_threshold = kTestBlinkThreshold;
    args->private_footprint_threshold = kTestPMFThreshold;
    args->swap_threshold = kTestSwapThreshold;
    args->virtual_memory_thresold = kTestVmSizeThreshold;

    intervention_->StartDetection(std::move(host_ptr), std::move(args),
                                  renderer_pause_enabled, navigate_ads_enabled);
    test::RunDelayedTasks(TimeDelta::FromSeconds(1));
  }
