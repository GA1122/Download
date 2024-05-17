StatusBubble* Browser::GetStatusBubble() {
  if (chrome::IsRunningInAppMode())
    return NULL;

  return window_ ? window_->GetStatusBubble() : NULL;
}
