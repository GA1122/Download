void GLES2DecoderImpl::ProcessPendingReadPixels() {
  while (!pending_readpixel_fences_.empty() &&
         pending_readpixel_fences_.front()->fence->HasCompleted()) {
    std::vector<base::Closure> callbacks =
        pending_readpixel_fences_.front()->callbacks;
    pending_readpixel_fences_.pop();
    for (size_t i = 0; i < callbacks.size(); i++) {
      callbacks[i].Run();
    }
  }
}
