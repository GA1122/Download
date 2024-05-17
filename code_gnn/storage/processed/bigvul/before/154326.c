void GLES2DecoderImpl::ProcessPendingReadPixels(bool did_finish) {
  while (!pending_readpixel_fences_.empty() &&
         (did_finish ||
          pending_readpixel_fences_.front().fence->HasCompleted())) {
    std::vector<base::OnceClosure> callbacks =
        std::move(pending_readpixel_fences_.front().callbacks);
    pending_readpixel_fences_.pop();
    for (size_t i = 0; i < callbacks.size(); i++) {
      std::move(callbacks[i]).Run();
    }
  }
}
