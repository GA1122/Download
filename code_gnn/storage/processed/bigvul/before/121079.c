scoped_ptr<base::Value> PixelBufferRasterWorkerPool::ThrottleStateAsValue()
    const {
  scoped_ptr<base::DictionaryValue> throttle_state(new base::DictionaryValue);

  throttle_state->SetInteger("bytes_available_for_upload",
                             max_bytes_pending_upload_ - bytes_pending_upload_);
  throttle_state->SetInteger("bytes_pending_upload", bytes_pending_upload_);
  throttle_state->SetInteger("scheduled_raster_task_count",
                             scheduled_raster_task_count_);
  return throttle_state.PassAs<base::Value>();
}
