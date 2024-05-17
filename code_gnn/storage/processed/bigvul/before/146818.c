int Document::RequestAnimationFrame(FrameRequestCallback* callback) {
  return EnsureScriptedAnimationController().RegisterCallback(callback);
}
