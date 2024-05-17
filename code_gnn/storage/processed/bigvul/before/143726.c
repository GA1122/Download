void RenderWidgetHostImpl::WindowSnapshotReachedScreen(int snapshot_id) {
  DCHECK(base::MessageLoopForUI::IsCurrent());

  gfx::Rect view_bounds = GetView()->GetViewBounds();
  gfx::Rect snapshot_bounds(view_bounds.size());

  std::vector<unsigned char> png;
  if (ui::GrabViewSnapshot(
      GetView()->GetNativeView(), &png, snapshot_bounds)) {
    OnSnapshotDataReceived(snapshot_id, &png.front(), png.size());
    return;
  }

  ui::GrabViewSnapshotAsync(
      GetView()->GetNativeView(),
      snapshot_bounds,
      base::ThreadTaskRunnerHandle::Get(),
      base::Bind(&RenderWidgetHostImpl::OnSnapshotDataReceivedAsync,
                 weak_factory_.GetWeakPtr(),
                 snapshot_id));
}
