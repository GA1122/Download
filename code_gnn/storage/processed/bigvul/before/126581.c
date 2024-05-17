void TabStripGtk::StartRemoveTabAnimation(int index, WebContents* contents) {
  if (active_animation_.get()) {
    active_animation_->set_layout_on_completion(false);
    active_animation_->Stop();
  }

  active_animation_.reset(new RemoveTabAnimation(this, index, contents));
  active_animation_->Start();
}
