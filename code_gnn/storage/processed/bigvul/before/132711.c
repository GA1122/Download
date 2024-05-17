void ChromotingInstance::DidChangeView(const pp::View& view) {
  DCHECK(plugin_task_runner_->BelongsToCurrentThread());

  plugin_view_ = view;
  mouse_input_filter_.set_input_size(
      webrtc::DesktopSize(view.GetRect().width(), view.GetRect().height()));

  if (video_renderer_)
    video_renderer_->OnViewChanged(view);
}
