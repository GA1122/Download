GLES2Implementation::GLES2Implementation(
    GLES2CmdHelper* helper,
    scoped_refptr<ShareGroup> share_group,
    TransferBufferInterface* transfer_buffer,
    bool bind_generates_resource,
    bool lose_context_when_out_of_memory,
    bool support_client_side_arrays,
    GpuControl* gpu_control)
    : ImplementationBase(helper, transfer_buffer, gpu_control),
      helper_(helper),
      chromium_framebuffer_multisample_(kUnknownExtensionStatus),
      pack_alignment_(4),
      pack_row_length_(0),
      pack_skip_pixels_(0),
      pack_skip_rows_(0),
      unpack_alignment_(4),
      unpack_row_length_(0),
      unpack_image_height_(0),
      unpack_skip_rows_(0),
      unpack_skip_pixels_(0),
      unpack_skip_images_(0),
      active_texture_unit_(0),
      bound_framebuffer_(0),
      bound_read_framebuffer_(0),
      bound_renderbuffer_(0),
      current_program_(0),
      bound_array_buffer_(0),
      bound_atomic_counter_buffer_(0),
      bound_copy_read_buffer_(0),
      bound_copy_write_buffer_(0),
      bound_pixel_pack_buffer_(0),
      bound_pixel_unpack_buffer_(0),
      bound_shader_storage_buffer_(0),
      bound_transform_feedback_buffer_(0),
      bound_uniform_buffer_(0),
      bound_pixel_pack_transfer_buffer_id_(0),
      bound_pixel_unpack_transfer_buffer_id_(0),
      error_bits_(0),
      lose_context_when_out_of_memory_(lose_context_when_out_of_memory),
      support_client_side_arrays_(support_client_side_arrays),
      use_count_(0),
      flush_id_(0),
      max_extra_transfer_buffer_size_(0),
      current_trace_stack_(0),
      aggressively_free_resources_(false),
      cached_extension_string_(nullptr),
      weak_ptr_factory_(this) {
  DCHECK(helper);

  std::stringstream ss;
  ss << std::hex << this;
  this_in_hex_ = ss.str();

  share_group_ =
      (share_group ? std::move(share_group)
                   : new ShareGroup(
                         bind_generates_resource,
                         gpu_control_->GetCommandBufferID().GetUnsafeValue()));
  DCHECK(share_group_->bind_generates_resource() == bind_generates_resource);

  memset(&reserved_ids_, 0, sizeof(reserved_ids_));
}
