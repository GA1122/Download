void GLES2DecoderImpl::WaitForReadPixels(base::OnceClosure callback) {
  if (features().use_async_readpixels && !pending_readpixel_fences_.empty()) {
    pending_readpixel_fences_.back().callbacks.push_back(std::move(callback));
  } else {
    std::move(callback).Run();
  }
}
