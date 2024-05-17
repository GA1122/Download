void GLES2DecoderImpl::WaitForReadPixels(base::Closure callback) {
  if (features().use_async_readpixels && !pending_readpixel_fences_.empty()) {
    pending_readpixel_fences_.back()->callbacks.push_back(callback);
  } else {
    callback.Run();
  }
}
