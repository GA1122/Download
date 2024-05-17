void RenderViewHostImpl::OnMediaNotification(int64 player_cookie,
                                             bool has_video,
                                             bool has_audio,
                                             bool is_playing) {
  if (is_playing) {
    scoped_ptr<PowerSaveBlocker> blocker;
    if (has_video) {
      blocker = PowerSaveBlocker::Create(
          PowerSaveBlocker::kPowerSaveBlockPreventDisplaySleep,
          "Playing video");
    } else if (has_audio) {
      blocker = PowerSaveBlocker::Create(
          PowerSaveBlocker::kPowerSaveBlockPreventAppSuspension,
          "Playing audio");
    }

    if (blocker)
      power_save_blockers_[player_cookie] = blocker.release();
  } else {
    delete power_save_blockers_[player_cookie];
    power_save_blockers_.erase(player_cookie);
  }
}
