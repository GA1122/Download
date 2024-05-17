gpu::ContextResult GLES2Implementation::Initialize(
    const SharedMemoryLimits& limits) {
  TRACE_EVENT0("gpu", "GLES2Implementation::Initialize");
  auto result = ImplementationBase::Initialize(limits);
  if (result != gpu::ContextResult::kSuccess) {
    return result;
  }

  max_extra_transfer_buffer_size_ = limits.max_mapped_memory_for_texture_upload;

  GLStaticState::ShaderPrecisionMap* shader_precisions =
      &static_state_.shader_precisions;
  capabilities_.VisitPrecisions(
      [shader_precisions](GLenum shader, GLenum type,
                          Capabilities::ShaderPrecision* result) {
        const GLStaticState::ShaderPrecisionKey key(shader, type);
        cmds::GetShaderPrecisionFormat::Result cached_result = {
            true, result->min_range, result->max_range, result->precision};
        shader_precisions->insert(std::make_pair(key, cached_result));
      });

  util_.set_num_compressed_texture_formats(
      capabilities_.num_compressed_texture_formats);
  util_.set_num_shader_binary_formats(capabilities_.num_shader_binary_formats);

  texture_units_ = std::make_unique<TextureUnit[]>(
      capabilities_.max_combined_texture_image_units);

  buffer_tracker_ = std::make_unique<BufferTracker>(mapped_memory_.get());
  readback_buffer_shadow_tracker_ =
      std::make_unique<ReadbackBufferShadowTracker>(mapped_memory_.get(),
                                                    helper_);

  for (int i = 0; i < static_cast<int>(IdNamespaces::kNumIdNamespaces); ++i)
    id_allocators_[i].reset(new IdAllocator());

  if (support_client_side_arrays_) {
    GetIdHandler(SharedIdNamespaces::kBuffers)
        ->MakeIds(this, kClientSideArrayId, base::size(reserved_ids_),
                  &reserved_ids_[0]);
  }

  vertex_array_object_manager_.reset(new VertexArrayObjectManager(
      capabilities_.max_vertex_attribs, reserved_ids_[0], reserved_ids_[1],
      support_client_side_arrays_));

  if (capabilities_.bind_generates_resource_chromium !=
      (share_group_->bind_generates_resource() ? 1 : 0)) {
    SetGLError(GL_INVALID_OPERATION, "Initialize",
               "Service bind_generates_resource mismatch.");
    LOG(ERROR) << "ContextResult::kFatalFailure: "
               << "bind_generates_resource mismatch";
    return gpu::ContextResult::kFatalFailure;
  }

  return gpu::ContextResult::kSuccess;
}
