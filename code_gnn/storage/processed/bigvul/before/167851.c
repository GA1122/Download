FrameLoader::FrameLoader(LocalFrame* frame)
    : frame_(frame),
      progress_tracker_(ProgressTracker::Create(frame)),
      in_stop_all_loaders_(false),
      in_restore_scroll_(false),
      forced_sandbox_flags_(kSandboxNone),
      dispatching_did_clear_window_object_in_main_world_(false),
      protect_provisional_loader_(false),
      detached_(false) {
  DCHECK(frame_);

  TRACE_EVENT_OBJECT_CREATED_WITH_ID("loading", "FrameLoader", this);
  TakeObjectSnapshot();
}
