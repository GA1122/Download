void RenderViewHostImpl::OnGetWindowSnapshot(const int snapshot_id) {
  std::vector<unsigned char> png;

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();
  if (command_line.HasSwitch(switches::kEnableGpuBenchmarking)) {
    gfx::Rect view_bounds = GetView()->GetViewBounds();
    gfx::Rect snapshot_bounds(view_bounds.size());
    gfx::Size snapshot_size = snapshot_bounds.size();

    if (ui::GrabViewSnapshot(GetView()->GetNativeView(),
                             &png, snapshot_bounds)) {
      Send(new ViewMsg_WindowSnapshotCompleted(
          GetRoutingID(), snapshot_id, snapshot_size, png));
      return;
    }
  }

  Send(new ViewMsg_WindowSnapshotCompleted(
      GetRoutingID(), snapshot_id, gfx::Size(), png));
}
