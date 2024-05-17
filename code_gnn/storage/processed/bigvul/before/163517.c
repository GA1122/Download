void PushMessagingServiceImpl::SetContentSettingChangedCallbackForTesting(
    const base::Closure& callback) {
  content_setting_changed_callback_for_testing_ = callback;
}
