  void ExtendSubframeUnloadTimeoutForTerminationPing(RenderFrameHostImpl* rfh) {
    rfh->SetSubframeUnloadTimeoutForTesting(base::TimeDelta::FromSeconds(30));
  }
