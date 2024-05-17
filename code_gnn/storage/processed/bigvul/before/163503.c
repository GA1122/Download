void PushMessagingServiceImpl::IncreasePushSubscriptionCount(int add,
                                                             bool is_pending) {
  DCHECK_GT(add, 0);
  if (push_subscription_count_ + pending_push_subscription_count_ == 0) {
    GetGCMDriver()->AddAppHandler(kPushMessagingAppIdentifierPrefix, this);
  }
  if (is_pending) {
    pending_push_subscription_count_ += add;
  } else {
#if BUILDFLAG(ENABLE_BACKGROUND)
    if (UseBackgroundMode() && g_browser_process->background_mode_manager() &&
        !push_subscription_count_) {
      g_browser_process->background_mode_manager()->RegisterTrigger(
          profile_, this, false  );
    }
#endif   
    push_subscription_count_ += add;
  }
}
