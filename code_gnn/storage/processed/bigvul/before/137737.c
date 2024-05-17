void HeadlessWebContentsImpl::BeginFrame(
    const base::TimeTicks& frame_timeticks,
    const base::TimeTicks& deadline,
    const base::TimeDelta& interval,
    bool capture_screenshot,
    const FrameFinishedCallback& frame_finished_callback) {
  DCHECK(begin_frame_control_enabled_);
  TRACE_EVENT2("headless", "HeadlessWebContentsImpl::BeginFrame", "frame_time",
               frame_timeticks, "capture_screenshot", capture_screenshot);

  uint64_t sequence_number = begin_frame_sequence_number_++;

  auto pending_frame = base::MakeUnique<PendingFrame>();
  pending_frame->sequence_number = sequence_number;
  pending_frame->callback = frame_finished_callback;

  if (capture_screenshot) {
    pending_frame->wait_for_copy_result = true;
    content::RenderWidgetHostView* view =
        web_contents()->GetRenderWidgetHostView();
    if (view) {
      view->CopyFromSurface(
          gfx::Rect(), gfx::Size(),
          base::Bind(&HeadlessWebContentsImpl::PendingFrameReadbackComplete,
                     base::Unretained(this),
                     base::Unretained(pending_frame.get())),
          kN32_SkColorType);
    }
  }

  pending_frames_.push_back(std::move(pending_frame));

  ui::Compositor* compositor = browser()->PlatformGetCompositor(this);
  DCHECK(compositor);

  compositor->IssueExternalBeginFrame(viz::BeginFrameArgs::Create(
      BEGINFRAME_FROM_HERE, begin_frame_source_id_, sequence_number,
      frame_timeticks, deadline, interval, viz::BeginFrameArgs::NORMAL));
}
