void LayerTreeHost::RateLimit() {
  proxy_->ForceSerializeOnSwapBuffers();
  client_->RateLimitSharedMainThreadContext();
}
