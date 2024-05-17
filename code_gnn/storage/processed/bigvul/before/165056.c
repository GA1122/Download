void OffscreenCanvas::Commit(scoped_refptr<CanvasResource> canvas_resource,
                             const SkIRect& damage_rect) {
  if (!HasPlaceholderCanvas() || !canvas_resource)
    return;
  RecordCanvasSizeToUMA(
      Size(), CanvasRenderingContextHost::HostType::kOffscreenCanvasHost);

  base::TimeTicks commit_start_time = WTF::CurrentTimeTicks();
  current_frame_damage_rect_.join(damage_rect);
  GetOrCreateResourceDispatcher()->DispatchFrameSync(
      std::move(canvas_resource), commit_start_time, current_frame_damage_rect_,
      !RenderingContext()->IsOriginTopLeft()  ,
      IsOpaque());
  current_frame_damage_rect_ = SkIRect::MakeEmpty();
 }
