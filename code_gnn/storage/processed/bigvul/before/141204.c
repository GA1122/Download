int Document::RequestAnimationFrame(
    FrameRequestCallbackCollection::FrameCallback* callback) {
  return EnsureScriptedAnimationController().RegisterCallback(callback);
}
