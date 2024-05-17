 void RenderWidgetHostViewAura::RunCompositingDidCommitCallbacks(
    ui::Compositor* compositor) {
  for (std::vector< base::Callback<void(ui::Compositor*)> >::const_iterator
// void RenderWidgetHostViewAura::RunCompositingDidCommitCallbacks() {
//   for (std::vector<base::Closure>::const_iterator
        it = on_compositing_did_commit_callbacks_.begin();
        it != on_compositing_did_commit_callbacks_.end(); ++it) {
    it->Run(compositor);
//     it->Run();
    }
    on_compositing_did_commit_callbacks_.clear();
  }