error::Error GLES2DecoderImpl::HandleScheduleCALayerSharedStateCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::ScheduleCALayerSharedStateCHROMIUM& c =
      *static_cast<
          const volatile gles2::cmds::ScheduleCALayerSharedStateCHROMIUM*>(
          cmd_data);

  const GLfloat* mem = GetSharedMemoryAs<const GLfloat*>(c.shm_id, c.shm_offset,
                                                         20 * sizeof(GLfloat));
  if (!mem) {
    return error::kOutOfBounds;
  }
  gfx::RectF clip_rect(mem[0], mem[1], mem[2], mem[3]);
  gfx::Transform transform(mem[4], mem[8], mem[12], mem[16],
                           mem[5], mem[9], mem[13], mem[17],
                           mem[6], mem[10], mem[14], mem[18],
                           mem[7], mem[11], mem[15], mem[19]);
  ca_layer_shared_state_.reset(new CALayerSharedState);
  ca_layer_shared_state_->opacity = c.opacity;
  ca_layer_shared_state_->is_clipped = c.is_clipped ? true : false;
  ca_layer_shared_state_->clip_rect = gfx::ToEnclosingRect(clip_rect);
  ca_layer_shared_state_->sorting_context_id = c.sorting_context_id;
  ca_layer_shared_state_->transform = transform;
  return error::kNoError;
}
