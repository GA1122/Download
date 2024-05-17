void OffscreenCanvas::PushFrame(scoped_refptr<CanvasResource> canvas_resource,
                                const SkIRect& damage_rect) {
  DCHECK(needs_push_frame_);
  needs_push_frame_ = false;
  current_frame_damage_rect_.join(damage_rect);
  if (current_frame_damage_rect_.isEmpty() || !canvas_resource)
    return;
  const base::TimeTicks commit_start_time = WTF::CurrentTimeTicks();
  GetOrCreateResourceDispatcher()->DispatchFrame(
      std::move(canvas_resource), commit_start_time, current_frame_damage_rect_,
      !RenderingContext()->IsOriginTopLeft()  ,
      IsOpaque());
  current_frame_damage_rect_ = SkIRect::MakeEmpty();
}
