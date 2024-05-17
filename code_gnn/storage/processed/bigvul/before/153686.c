bool GLES2Implementation::GetHelper(GLenum pname, GLint* params) {

  switch (pname) {
    case GL_ACTIVE_TEXTURE:
      *params = active_texture_unit_ + GL_TEXTURE0;
      return true;
    case GL_ARRAY_BUFFER_BINDING:
      *params = bound_array_buffer_;
      return true;
    case GL_ELEMENT_ARRAY_BUFFER_BINDING:
      *params = vertex_array_object_manager_->bound_element_array_buffer();
      return true;
    case GL_FRAMEBUFFER_BINDING:
      *params = bound_framebuffer_;
      return true;
    case GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS:
      *params = capabilities_.max_combined_texture_image_units;
      return true;
    case GL_MAX_CUBE_MAP_TEXTURE_SIZE:
      *params = capabilities_.max_cube_map_texture_size;
      return true;
    case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
      *params = capabilities_.max_fragment_uniform_vectors;
      return true;
    case GL_MAX_RENDERBUFFER_SIZE:
      *params = capabilities_.max_renderbuffer_size;
      return true;
    case GL_MAX_TEXTURE_IMAGE_UNITS:
      *params = capabilities_.max_texture_image_units;
      return true;
    case GL_MAX_TEXTURE_SIZE:
      *params = capabilities_.max_texture_size;
      return true;
    case GL_MAX_VARYING_VECTORS:
      *params = capabilities_.max_varying_vectors;
      return true;
    case GL_MAX_VERTEX_ATTRIBS:
      *params = capabilities_.max_vertex_attribs;
      return true;
    case GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS:
      *params = capabilities_.max_vertex_texture_image_units;
      return true;
    case GL_MAX_VERTEX_UNIFORM_VECTORS:
      *params = capabilities_.max_vertex_uniform_vectors;
      return true;
    case GL_MAX_VIEWPORT_DIMS:
      if (capabilities_.max_viewport_width > 0 &&
          capabilities_.max_viewport_height > 0) {
        params[0] = capabilities_.max_viewport_width;
        params[1] = capabilities_.max_viewport_height;
        return true;
      }
      return false;
    case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
      *params = capabilities_.num_compressed_texture_formats;
      return true;
    case GL_NUM_SHADER_BINARY_FORMATS:
      *params = capabilities_.num_shader_binary_formats;
      return true;
    case GL_RENDERBUFFER_BINDING:
      *params = bound_renderbuffer_;
      return true;
    case GL_TEXTURE_BINDING_2D:
      *params = texture_units_[active_texture_unit_].bound_texture_2d;
      return true;
    case GL_TEXTURE_BINDING_CUBE_MAP:
      *params = texture_units_[active_texture_unit_].bound_texture_cube_map;
      return true;

    case GL_TEXTURE_BINDING_EXTERNAL_OES:
      *params = texture_units_[active_texture_unit_].bound_texture_external_oes;
      return true;
    case GL_TEXTURE_BINDING_RECTANGLE_ARB:
      *params =
          texture_units_[active_texture_unit_].bound_texture_rectangle_arb;
      return true;
    case GL_PIXEL_PACK_TRANSFER_BUFFER_BINDING_CHROMIUM:
      *params = bound_pixel_pack_transfer_buffer_id_;
      return true;
    case GL_PIXEL_UNPACK_TRANSFER_BUFFER_BINDING_CHROMIUM:
      *params = bound_pixel_unpack_transfer_buffer_id_;
      return true;
    case GL_READ_FRAMEBUFFER_BINDING:
      if (capabilities_.major_version >= 3 ||
          IsChromiumFramebufferMultisampleAvailable()) {
        *params = bound_read_framebuffer_;
        return true;
      }
      break;
    case GL_TIMESTAMP_EXT:
      *params = base::saturated_cast<GLint>(
          (base::TimeTicks::Now() - base::TimeTicks()).InMicroseconds() *
          base::Time::kNanosecondsPerMicrosecond);
      return true;
    case GL_GPU_DISJOINT_EXT:
      *params = static_cast<GLint>(query_tracker_->CheckAndResetDisjoint());
      return true;

    case GL_VIEWPORT:
      if (state_.viewport_width > 0 && state_.viewport_height > 0 &&
          capabilities_.max_viewport_width > 0 &&
          capabilities_.max_viewport_height > 0) {
        params[0] = state_.viewport_x;
        params[1] = state_.viewport_y;
        params[2] =
            std::min(state_.viewport_width, capabilities_.max_viewport_width);
        params[3] =
            std::min(state_.viewport_height, capabilities_.max_viewport_height);
        return true;
      }
      return false;

    case GL_ALIASED_LINE_WIDTH_RANGE:
    case GL_ALIASED_POINT_SIZE_RANGE:
    case GL_ALPHA_BITS:
    case GL_BLEND:
    case GL_BLEND_COLOR:
    case GL_BLEND_DST_ALPHA:
    case GL_BLEND_DST_RGB:
    case GL_BLEND_EQUATION_ALPHA:
    case GL_BLEND_EQUATION_RGB:
    case GL_BLEND_SRC_ALPHA:
    case GL_BLEND_SRC_RGB:
    case GL_BLUE_BITS:
    case GL_COLOR_CLEAR_VALUE:
    case GL_COLOR_WRITEMASK:
    case GL_COMPRESSED_TEXTURE_FORMATS:
    case GL_CULL_FACE:
    case GL_CULL_FACE_MODE:
    case GL_CURRENT_PROGRAM:
    case GL_DEPTH_BITS:
    case GL_DEPTH_CLEAR_VALUE:
    case GL_DEPTH_FUNC:
    case GL_DEPTH_RANGE:
    case GL_DEPTH_TEST:
    case GL_DEPTH_WRITEMASK:
    case GL_DITHER:
    case GL_FRONT_FACE:
    case GL_GENERATE_MIPMAP_HINT:
    case GL_GREEN_BITS:
    case GL_IMPLEMENTATION_COLOR_READ_FORMAT:
    case GL_IMPLEMENTATION_COLOR_READ_TYPE:
    case GL_LINE_WIDTH:
    case GL_PACK_ALIGNMENT:
    case GL_POLYGON_OFFSET_FACTOR:
    case GL_POLYGON_OFFSET_FILL:
    case GL_POLYGON_OFFSET_UNITS:
    case GL_RED_BITS:
    case GL_SAMPLE_ALPHA_TO_COVERAGE:
    case GL_SAMPLE_BUFFERS:
    case GL_SAMPLE_COVERAGE:
    case GL_SAMPLE_COVERAGE_INVERT:
    case GL_SAMPLE_COVERAGE_VALUE:
    case GL_SAMPLES:
    case GL_SCISSOR_BOX:
    case GL_SCISSOR_TEST:
    case GL_SHADER_BINARY_FORMATS:
    case GL_SHADER_COMPILER:
    case GL_STENCIL_BACK_FAIL:
    case GL_STENCIL_BACK_FUNC:
    case GL_STENCIL_BACK_PASS_DEPTH_FAIL:
    case GL_STENCIL_BACK_PASS_DEPTH_PASS:
    case GL_STENCIL_BACK_REF:
    case GL_STENCIL_BACK_VALUE_MASK:
    case GL_STENCIL_BACK_WRITEMASK:
    case GL_STENCIL_BITS:
    case GL_STENCIL_CLEAR_VALUE:
    case GL_STENCIL_FAIL:
    case GL_STENCIL_FUNC:
    case GL_STENCIL_PASS_DEPTH_FAIL:
    case GL_STENCIL_PASS_DEPTH_PASS:
    case GL_STENCIL_REF:
    case GL_STENCIL_TEST:
    case GL_STENCIL_VALUE_MASK:
    case GL_STENCIL_WRITEMASK:
    case GL_SUBPIXEL_BITS:
    case GL_UNPACK_ALIGNMENT:
      return false;
    default:
      break;
  }

  if (capabilities_.major_version < 3) {
    return false;
  }

  switch (pname) {
    case GL_COPY_READ_BUFFER_BINDING:
      *params = bound_copy_read_buffer_;
      return true;
    case GL_COPY_WRITE_BUFFER_BINDING:
      *params = bound_copy_write_buffer_;
      return true;
    case GL_MAJOR_VERSION:
      *params = capabilities_.major_version;
      return true;
    case GL_MAX_3D_TEXTURE_SIZE:
      *params = capabilities_.max_3d_texture_size;
      return true;
    case GL_MAX_ARRAY_TEXTURE_LAYERS:
      *params = capabilities_.max_array_texture_layers;
      return true;
    case GL_MAX_COLOR_ATTACHMENTS:
      *params = capabilities_.max_color_attachments;
      return true;
    case GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS:
      *params = static_cast<GLint>(
          capabilities_.max_combined_fragment_uniform_components);
      return true;
    case GL_MAX_COMBINED_UNIFORM_BLOCKS:
      *params = capabilities_.max_combined_uniform_blocks;
      return true;
    case GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS:
      *params = static_cast<GLint>(
          capabilities_.max_combined_vertex_uniform_components);
      return true;
    case GL_MAX_DRAW_BUFFERS:
      *params = capabilities_.max_draw_buffers;
      return true;
    case GL_MAX_ELEMENT_INDEX:
      *params = static_cast<GLint>(capabilities_.max_element_index);
      return true;
    case GL_MAX_ELEMENTS_INDICES:
      *params = capabilities_.max_elements_indices;
      return true;
    case GL_MAX_ELEMENTS_VERTICES:
      *params = capabilities_.max_elements_vertices;
      return true;
    case GL_MAX_FRAGMENT_INPUT_COMPONENTS:
      *params = capabilities_.max_fragment_input_components;
      return true;
    case GL_MAX_FRAGMENT_UNIFORM_BLOCKS:
      *params = capabilities_.max_fragment_uniform_blocks;
      return true;
    case GL_MAX_FRAGMENT_UNIFORM_COMPONENTS:
      *params = capabilities_.max_fragment_uniform_components;
      return true;
    case GL_MAX_PROGRAM_TEXEL_OFFSET:
      *params = capabilities_.max_program_texel_offset;
      return true;
    case GL_MAX_SAMPLES:
      *params = capabilities_.max_samples;
      return true;
    case GL_MAX_SERVER_WAIT_TIMEOUT:
      *params = static_cast<GLint>(capabilities_.max_server_wait_timeout);
      return true;
    case GL_MAX_TEXTURE_LOD_BIAS:
      *params = static_cast<GLint>(capabilities_.max_texture_lod_bias);
      return true;
    case GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS:
      *params = capabilities_.max_transform_feedback_interleaved_components;
      return true;
    case GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS:
      *params = capabilities_.max_transform_feedback_separate_attribs;
      return true;
    case GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS:
      *params = capabilities_.max_transform_feedback_separate_components;
      return true;
    case GL_MAX_UNIFORM_BLOCK_SIZE:
      *params = static_cast<GLint>(capabilities_.max_uniform_block_size);
      return true;
    case GL_MAX_UNIFORM_BUFFER_BINDINGS:
      *params = capabilities_.max_uniform_buffer_bindings;
      return true;
    case GL_MAX_VARYING_COMPONENTS:
      *params = capabilities_.max_varying_components;
      return true;
    case GL_MAX_VERTEX_OUTPUT_COMPONENTS:
      *params = capabilities_.max_vertex_output_components;
      return true;
    case GL_MAX_VERTEX_UNIFORM_BLOCKS:
      *params = capabilities_.max_vertex_uniform_blocks;
      return true;
    case GL_MAX_VERTEX_UNIFORM_COMPONENTS:
      *params = capabilities_.max_vertex_uniform_components;
      return true;
    case GL_MIN_PROGRAM_TEXEL_OFFSET:
      *params = capabilities_.min_program_texel_offset;
      return true;
    case GL_MINOR_VERSION:
      *params = capabilities_.minor_version;
      return true;
    case GL_NUM_EXTENSIONS:
      UpdateCachedExtensionsIfNeeded();
      *params = cached_extensions_.size();
      return true;
    case GL_NUM_PROGRAM_BINARY_FORMATS:
      *params = capabilities_.num_program_binary_formats;
      return true;
    case GL_PACK_SKIP_PIXELS:
      *params = pack_skip_pixels_;
      return true;
    case GL_PACK_SKIP_ROWS:
      *params = pack_skip_rows_;
      return true;
    case GL_PIXEL_PACK_BUFFER_BINDING:
      *params = bound_pixel_pack_buffer_;
      return true;
    case GL_PIXEL_UNPACK_BUFFER_BINDING:
      *params = bound_pixel_unpack_buffer_;
      return true;
    case GL_TRANSFORM_FEEDBACK_BUFFER_BINDING:
      *params = bound_transform_feedback_buffer_;
      return true;
    case GL_UNIFORM_BUFFER_BINDING:
      *params = bound_uniform_buffer_;
      return true;
    case GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT:
      *params = capabilities_.uniform_buffer_offset_alignment;
      return true;
    case GL_UNPACK_SKIP_IMAGES:
      *params = unpack_skip_images_;
      return true;
    case GL_UNPACK_SKIP_PIXELS:
      *params = unpack_skip_pixels_;
      return true;
    case GL_UNPACK_SKIP_ROWS:
      *params = unpack_skip_rows_;
      return true;

    case GL_DRAW_BUFFER0:
    case GL_DRAW_BUFFER1:
    case GL_DRAW_BUFFER2:
    case GL_DRAW_BUFFER3:
    case GL_DRAW_BUFFER4:
    case GL_DRAW_BUFFER5:
    case GL_DRAW_BUFFER6:
    case GL_DRAW_BUFFER7:
    case GL_DRAW_BUFFER8:
    case GL_DRAW_BUFFER9:
    case GL_DRAW_BUFFER10:
    case GL_DRAW_BUFFER11:
    case GL_DRAW_BUFFER12:
    case GL_DRAW_BUFFER13:
    case GL_DRAW_BUFFER14:
    case GL_DRAW_BUFFER15:
    case GL_DRAW_FRAMEBUFFER_BINDING:
    case GL_FRAGMENT_SHADER_DERIVATIVE_HINT:
    case GL_PACK_ROW_LENGTH:
    case GL_PRIMITIVE_RESTART_FIXED_INDEX:
    case GL_PROGRAM_BINARY_FORMATS:
    case GL_RASTERIZER_DISCARD:
    case GL_READ_BUFFER:
    case GL_READ_FRAMEBUFFER_BINDING:
    case GL_SAMPLER_BINDING:
    case GL_TEXTURE_BINDING_2D_ARRAY:
    case GL_TEXTURE_BINDING_3D:
    case GL_TRANSFORM_FEEDBACK_BINDING:
    case GL_TRANSFORM_FEEDBACK_ACTIVE:
    case GL_TRANSFORM_FEEDBACK_PAUSED:
    case GL_TRANSFORM_FEEDBACK_BUFFER_SIZE:
    case GL_TRANSFORM_FEEDBACK_BUFFER_START:
    case GL_UNIFORM_BUFFER_SIZE:
    case GL_UNIFORM_BUFFER_START:
    case GL_UNPACK_IMAGE_HEIGHT:
    case GL_UNPACK_ROW_LENGTH:
    case GL_VERTEX_ARRAY_BINDING:
      return false;
    default:
      break;
  }

  if (capabilities_.minor_version < 1) {
    return false;
  }

  switch (pname) {
    case GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS:
      *params = capabilities_.max_atomic_counter_buffer_bindings;
      return true;
    case GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS:
      *params = capabilities_.max_shader_storage_buffer_bindings;
      return true;
    case GL_ATOMIC_COUNTER_BUFFER_BINDING:
      *params = bound_atomic_counter_buffer_;
      return true;
    case GL_SHADER_STORAGE_BUFFER_BINDING:
      *params = bound_shader_storage_buffer_;
      return true;
    case GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT:
      *params = capabilities_.shader_storage_buffer_offset_alignment;
      return true;

    case GL_ATOMIC_COUNTER_BUFFER_SIZE:
    case GL_ATOMIC_COUNTER_BUFFER_START:
    case GL_SHADER_STORAGE_BUFFER_SIZE:
    case GL_SHADER_STORAGE_BUFFER_START:
      return false;
    default:
      return false;
  }
}
