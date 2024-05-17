bool WebLocalFrameImpl::IsLoading() const {
  if (!GetFrame() || !GetFrame()->GetDocument())
    return false;
  return GetFrame()
             ->Loader()
             .StateMachine()
             ->IsDisplayingInitialEmptyDocument() ||
         GetFrame()->Loader().HasProvisionalNavigation() ||
         !GetFrame()->GetDocument()->LoadEventFinished();
}
