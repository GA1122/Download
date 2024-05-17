void Document::WillInsertBody() {
  if (GetFrame())
    GetFrame()->Client()->DispatchWillInsertBody();
  BeginLifecycleUpdatesIfRenderingReady();
}
