void FrameLoader::DispatchDidClearDocumentOfWindowObject() {
  DCHECK(frame_->GetDocument());
  if (state_machine_.CreatingInitialEmptyDocument())
    return;

  Settings* settings = frame_->GetSettings();
  if (settings && settings->GetForceMainWorldInitialization()) {
    frame_->GetScriptController().WindowProxy(DOMWrapperWorld::MainWorld());
  }
  probe::DidClearDocumentOfWindowObject(frame_);
  if (!frame_->GetDocument()->CanExecuteScripts(kNotAboutToExecuteScript))
    return;

  if (dispatching_did_clear_window_object_in_main_world_)
    return;
  base::AutoReset<bool> in_did_clear_window_object(
      &dispatching_did_clear_window_object_in_main_world_, true);
  Client()->DispatchDidClearWindowObjectInMainWorld();
}
