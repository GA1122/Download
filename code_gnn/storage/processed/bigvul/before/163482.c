void PushMessagingServiceImpl::DecreasePushSubscriptionCount(int subtract,
                                                             bool was_pending) {
  DCHECK_GT(subtract, 0);
  if (was_pending) {
    pending_push_subscription_count_ -= subtract;
    DCHECK_GE(pending_push_subscription_count_, 0);
  } else {
    push_subscription_count_ -= subtract;
    DCHECK_GE(push_subscription_count_, 0);
  }
  if (push_subscription_count_ + pending_push_subscription_count_ == 0) {
    GetGCMDriver()->RemoveAppHandler(kPushMessagingAppIdentifierPrefix);

#if BUILDFLAG(ENABLE_BACKGROUND)
    if (UseBackgroundMode() && g_browser_process->background_mode_manager()) {
      g_browser_process->background_mode_manager()->UnregisterTrigger(profile_,
                                                                      this);
    }
#endif   
  }
}
