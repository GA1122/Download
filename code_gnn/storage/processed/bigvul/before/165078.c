void OffscreenCanvas::SetPlaceholderCanvasId(DOMNodeId canvas_id) {
  placeholder_canvas_id_ = canvas_id;
  if (GetTopExecutionContext() &&
      GetTopExecutionContext()->IsWorkerGlobalScope()) {
    WorkerAnimationFrameProvider* animation_frame_provider =
        To<WorkerGlobalScope>(GetTopExecutionContext())
            ->GetAnimationFrameProvider();
    if (animation_frame_provider)
      animation_frame_provider->RegisterOffscreenCanvas(this);
  }
}
