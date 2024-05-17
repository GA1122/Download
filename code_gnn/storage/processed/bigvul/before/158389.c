void RenderWidgetHostImpl::WindowSnapshotReachedScreen(int snapshot_id) {
  DCHECK(base::MessageLoopForUI::IsCurrent());

  if (!pending_surface_browser_snapshots_.empty()) {
    GetView()->CopyFromSurface(
        gfx::Rect(), gfx::Size(),
        base::BindOnce(&RenderWidgetHostImpl::OnSnapshotFromSurfaceReceived,
                       weak_factory_.GetWeakPtr(), snapshot_id, 0));
  }

  if (!pending_browser_snapshots_.empty()) {
#if defined(OS_ANDROID)
    gfx::Rect snapshot_bounds(GetView()->GetViewBounds());
    snapshot_bounds.Offset(0, GetView()->GetNativeView()->content_offset());
#else
    gfx::Rect snapshot_bounds(GetView()->GetViewBounds().size());
#endif

    gfx::Image image;
    if (ui::GrabViewSnapshot(GetView()->GetNativeView(), snapshot_bounds,
                             &image)) {
      OnSnapshotReceived(snapshot_id, image);
      return;
    }

    ui::GrabViewSnapshotAsync(
        GetView()->GetNativeView(), snapshot_bounds,
        base::Bind(&RenderWidgetHostImpl::OnSnapshotReceived,
                   weak_factory_.GetWeakPtr(), snapshot_id));
  }
}
