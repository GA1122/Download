void Document::EnqueueAnimationFrameTask(base::OnceClosure task) {
  EnsureScriptedAnimationController().EnqueueTask(std::move(task));
}
