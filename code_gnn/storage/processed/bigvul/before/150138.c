  void PostNextAfterDraw(LayerTreeHostImpl* host_impl) {
    if (posted_)
      return;
    posted_ = true;
    ImplThreadTaskRunner()->PostDelayedTask(
        FROM_HERE, base::Bind(&LayerTreeHostTestCrispUpAfterPinchEnds::Next,
                              base::Unretained(this), host_impl),
        base::TimeDelta::FromMilliseconds(16 * 4));
  }
