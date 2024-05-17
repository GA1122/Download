void ShellWindowViews::OnViewWasResized() {
#if defined(OS_WIN) && !defined(USE_AURA)
  DCHECK(window_);
  DCHECK(web_view_);
  gfx::Size sz = web_view_->size();
  int height = sz.height(), width = sz.width();
  int radius = 1;
  gfx::Path path;
  if (window_->IsMaximized() || window_->IsFullscreen()) {
    path.addRect(0, 0, width, height);
  } else {
    if (use_custom_frame_) {
      path.moveTo(0, radius);
      path.lineTo(radius, 0);
      path.lineTo(width - radius, 0);
      path.lineTo(width, radius);
    } else {
      path.moveTo(0, 0);
      path.lineTo(width, 0);
    }
    path.lineTo(width, height - radius - 1);
    path.lineTo(width - radius - 1, height);
    path.lineTo(radius + 1, height);
    path.lineTo(0, height - radius - 1);
    path.close();
  }
  SetWindowRgn(web_contents()->GetNativeView(), path.CreateNativeRegion(), 1);

  SkRegion* rgn = new SkRegion;
  if (!window_->IsFullscreen()) {
    if (caption_region_.Get())
      rgn->op(*caption_region_.Get(), SkRegion::kUnion_Op);
    if (!window_->IsMaximized()) {
      if (use_custom_frame_)
        rgn->op(0, 0, width, kResizeInsideBoundsSize, SkRegion::kUnion_Op);
      rgn->op(0, 0, kResizeInsideBoundsSize, height, SkRegion::kUnion_Op);
      rgn->op(width - kResizeInsideBoundsSize, 0, width, height,
          SkRegion::kUnion_Op);
      rgn->op(0, height - kResizeInsideBoundsSize, width, height,
          SkRegion::kUnion_Op);
    }
  }
  web_contents()->GetRenderViewHost()->GetView()->SetClickthroughRegion(rgn);
#endif
}
