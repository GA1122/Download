scoped_ptr<base::Value> PixelBufferRasterWorkerPool::StateAsValue() const {
  scoped_ptr<base::DictionaryValue> state(new base::DictionaryValue);

  state->SetInteger("completed_count", completed_tasks_.size());
  state->SetInteger("pending_count", pixel_buffer_tasks_.size());
  state->SetInteger("pending_upload_count", tasks_with_pending_upload_.size());
  state->SetInteger("required_for_activation_count",
                    tasks_required_for_activation_.size());
  state->Set("scheduled_state", ScheduledStateAsValue().release());
  state->Set("throttle_state", ThrottleStateAsValue().release());
  return state.PassAs<base::Value>();
}
