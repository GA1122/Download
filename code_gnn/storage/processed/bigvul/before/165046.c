void HTMLCanvasElement::UpdateMemoryUsage() {
  int non_gpu_buffer_count = 0;
  int gpu_buffer_count = 0;

  if (!Is2d() && !Is3d())
    return;
  if (ResourceProvider()) {
    non_gpu_buffer_count++;
    if (IsAccelerated()) {
      gpu_buffer_count += 2;
    }
  }

  if (Is3d())
    non_gpu_buffer_count += context_->ExternallyAllocatedBufferCountPerPixel();

  const int bytes_per_pixel = ColorParams().BytesPerPixel();

  if (gpu_buffer_count && !gpu_memory_usage_) {
    base::CheckedNumeric<intptr_t> checked_usage =
        gpu_buffer_count * bytes_per_pixel;
    checked_usage *= width();
    checked_usage *= height();
    intptr_t gpu_memory_usage =
        checked_usage.ValueOrDefault(std::numeric_limits<intptr_t>::max());

    global_gpu_memory_usage_ += (gpu_memory_usage - gpu_memory_usage_);
    gpu_memory_usage_ = gpu_memory_usage;
    global_accelerated_context_count_++;
  } else if (!gpu_buffer_count && gpu_memory_usage_) {
    DCHECK_GT(global_accelerated_context_count_, 0u);
    global_accelerated_context_count_--;
    global_gpu_memory_usage_ -= gpu_memory_usage_;
    gpu_memory_usage_ = 0;
  }

  base::CheckedNumeric<intptr_t> checked_usage =
      non_gpu_buffer_count * bytes_per_pixel;
  checked_usage *= width();
  checked_usage *= height();
  checked_usage += gpu_memory_usage_;
  intptr_t externally_allocated_memory =
      checked_usage.ValueOrDefault(std::numeric_limits<intptr_t>::max());
  v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(
      externally_allocated_memory - externally_allocated_memory_);
  externally_allocated_memory_ = externally_allocated_memory;
}
