  void RunMultipleWheelScroll() {
    DoSmoothWheelScroll(gfx::Vector2d(0, 100));
    while (!VerifyRecordedSamplesForHistogram(
        1, "Event.Latency.ScrollBegin.Wheel.TimeToScrollUpdateSwapBegin4")) {
      GiveItSomeTime();
      FetchHistogramsFromChildProcesses();
    }

    while (!VerifyRecordedSamplesForHistogram(
        1, "Event.Latency.ScrollUpdate.Wheel.TimeToScrollUpdateSwapBegin4")) {
      GiveItSomeTime();
      FetchHistogramsFromChildProcesses();
     }
   }
