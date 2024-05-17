void RenderWidgetHostImpl::OnForceRedrawComplete(int snapshot_id) {
#if defined(OS_MACOSX) || defined(OS_WIN)
  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE,
      base::BindOnce(&RenderWidgetHostImpl::WindowSnapshotReachedScreen,
                     weak_factory_.GetWeakPtr(), snapshot_id),
      TimeDelta::FromSecondsD(1. / 6));
#else
  WindowSnapshotReachedScreen(snapshot_id);
#endif
}
