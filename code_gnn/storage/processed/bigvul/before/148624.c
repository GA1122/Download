SkCanvas* SkiaOutputSurfaceImpl::BeginPaintCurrentFrame() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(!current_paint_);

  if (initialize_waitable_event_) {
    initialize_waitable_event_->Wait();
    initialize_waitable_event_ = nullptr;

    RecreateRootRecorder();
  }

  DCHECK(root_recorder_);

  current_paint_.emplace(&root_recorder_.value());

  if (!renderer_settings_.show_overdraw_feedback)
    return current_paint_->recorder()->getCanvas();

  DCHECK(!overdraw_surface_recorder_);
  DCHECK(renderer_settings_.show_overdraw_feedback);

  SkSurfaceCharacterization characterization = CreateSkSurfaceCharacterization(
      gfx::Size(characterization_.width(), characterization_.height()),
      BGRA_8888, false  , characterization_.refColorSpace());
  overdraw_surface_recorder_.emplace(characterization);
  overdraw_canvas_.emplace((overdraw_surface_recorder_->getCanvas()));

  nway_canvas_.emplace(characterization_.width(), characterization_.height());
  nway_canvas_->addCanvas(current_paint_->recorder()->getCanvas());
  nway_canvas_->addCanvas(&overdraw_canvas_.value());
  return &nway_canvas_.value();
}
