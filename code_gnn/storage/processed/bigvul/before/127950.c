void BrowserViewRenderer::PostFallbackTick() {
  DCHECK(fallback_tick_fired_.IsCancelled());
  TRACE_EVENT0("android_webview", "BrowserViewRenderer::PostFallbackTick");
  post_fallback_tick_.Cancel();
  fallback_tick_fired_.Reset(base::Bind(&BrowserViewRenderer::FallbackTickFired,
                                        base::Unretained(this)));
  ui_task_runner_->PostDelayedTask(
      FROM_HERE, fallback_tick_fired_.callback(),
      base::TimeDelta::FromMilliseconds(kFallbackTickTimeoutInMilliseconds));
}
