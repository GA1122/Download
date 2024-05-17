void ClientControlledShellSurface::SetExtraTitle(
    const base::string16& extra_title) {
  TRACE_EVENT1("exo", "ClientControlledShellSurface::SetExtraTitle",
               "extra_title", base::UTF16ToUTF8(extra_title));

  if (!widget_)
    return;

  GetFrameView()->GetHeaderView()->GetFrameHeader()->SetFrameTextOverride(
      extra_title);
  if (wide_frame_) {
    wide_frame_->header_view()->GetFrameHeader()->SetFrameTextOverride(
        extra_title);
  }
}
