void FrameLoader::DispatchDidClearWindowObjectInMainWorld() {
  DCHECK(frame_->GetDocument());
  if (!frame_->GetDocument()->CanExecuteScripts(kNotAboutToExecuteScript))
    return;

  if (dispatching_did_clear_window_object_in_main_world_)
    return;
  AutoReset<bool> in_did_clear_window_object(
      &dispatching_did_clear_window_object_in_main_world_, true);
  Client()->DispatchDidClearWindowObjectInMainWorld();
}
