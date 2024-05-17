void BrowserViewRenderer::PostInvalidateWithFallback() {
  client_->PostInvalidate();

  bool throttle_fallback_tick =
      (is_paused_ && !clear_view_) || (attached_to_window_ && !window_visible_);

  if (throttle_fallback_tick || fallback_tick_pending_)
    return;

  DCHECK(post_fallback_tick_.IsCancelled());
  DCHECK(fallback_tick_fired_.IsCancelled());

  post_fallback_tick_.Reset(base::Bind(&BrowserViewRenderer::PostFallbackTick,
                                       base::Unretained(this)));
  ui_task_runner_->PostTask(FROM_HERE, post_fallback_tick_.callback());
  fallback_tick_pending_ = true;
}
