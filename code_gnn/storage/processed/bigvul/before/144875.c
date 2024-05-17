   void TimeAdvance() {
     WebView().Scheduler()->SetVirtualTimePolicy(
         PageScheduler::VirtualTimePolicy::kAdvance);
  }
