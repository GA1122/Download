void TestRenderViewHost::OnWebkitPreferencesChanged() {
  RenderViewHostImpl::OnWebkitPreferencesChanged();
  if (webkit_preferences_changed_counter_)
    ++*webkit_preferences_changed_counter_;
}
