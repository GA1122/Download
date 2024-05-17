void ChromotingHost::EnableCurtainMode(bool enable) {
  if (is_it2me_ || enable == is_curtained_)
    return;
  desktop_environment_->curtain()->EnableCurtainMode(enable);
   is_curtained_ = enable;
 }
