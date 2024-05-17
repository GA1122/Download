void InspectorPageAgent::WillCommitLoad(LocalFrame*, DocumentLoader* loader) {
  if (loader->GetFrame() == inspected_frames_->Root()) {
    FinishReload();
    script_to_evaluate_on_load_once_ = pending_script_to_evaluate_on_load_once_;
    pending_script_to_evaluate_on_load_once_ = String();
  }
  GetFrontend()->lifecycleEvent(IdentifiersFactory::FrameId(loader->GetFrame()),
                                "commit", MonotonicallyIncreasingTime());
  GetFrontend()->frameNavigated(BuildObjectForFrame(loader->GetFrame()));
}
