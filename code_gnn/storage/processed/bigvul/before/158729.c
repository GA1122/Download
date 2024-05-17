error::Error GLES2DecoderImpl::HandleScheduleDCLayerSharedStateCHROMIUM(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::ScheduleDCLayerSharedStateCHROMIUM& c =
      *static_cast<
          const volatile gles2::cmds::ScheduleDCLayerSharedStateCHROMIUM*>(
          cmd_data);

  const GLfloat* mem = GetSharedMemoryAs<const GLfloat*>(c.shm_id, c.shm_offset,
                                                         20 * sizeof(GLfloat));
  if (!mem) {
    return error::kOutOfBounds;
  }
  gfx::RectF clip_rect(mem[0], mem[1], mem[2], mem[3]);
  gfx::Transform transform(mem[4], mem[8], mem[12], mem[16], mem[5], mem[9],
                           mem[13], mem[17], mem[6], mem[10], mem[14], mem[18],
                           mem[7], mem[11], mem[15], mem[19]);
  dc_layer_shared_state_.reset(new DCLayerSharedState);
  dc_layer_shared_state_->opacity = c.opacity;
  dc_layer_shared_state_->is_clipped = c.is_clipped ? true : false;
  dc_layer_shared_state_->clip_rect = gfx::ToEnclosingRect(clip_rect);
  dc_layer_shared_state_->z_order = c.z_order;
  dc_layer_shared_state_->transform = transform;
  return error::kNoError;
}
