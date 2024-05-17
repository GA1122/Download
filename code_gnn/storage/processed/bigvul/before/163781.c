void DelegatedFrameHost::OnUpdateVSyncParameters(base::TimeTicks timebase,
                                                 base::TimeDelta interval) {
  vsync_timebase_ = timebase;
  vsync_interval_ = interval;
}
