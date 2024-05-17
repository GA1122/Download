bool GLES2DecoderImpl::GetHelper(
    GLenum pname, GLint* params, GLsizei* num_written) {
  DCHECK(num_written);
  if (gfx::GetGLImplementation() != gfx::kGLImplementationEGLGLES2) {
    switch (pname) {
      case GL_IMPLEMENTATION_COLOR_READ_FORMAT:
        *num_written = 1;
        if (params) {
          if (context_->HasExtension("GL_OES_read_format")) {
            ScopedGLErrorSuppressor suppressor("GLES2DecoderImpl::GetHelper",
                                               GetErrorState());
            glGetIntegerv(pname, params);
            if (glGetError() == GL_NO_ERROR)
              return true;
          }
          *params = GLES2Util::GetPreferredGLReadPixelsFormat(
              GetBoundReadFrameBufferInternalFormat());
        }
        return true;
      case GL_IMPLEMENTATION_COLOR_READ_TYPE:
        *num_written = 1;
        if (params) {
          if (context_->HasExtension("GL_OES_read_format")) {
            ScopedGLErrorSuppressor suppressor("GLES2DecoderImpl::GetHelper",
                                               GetErrorState());
            glGetIntegerv(pname, params);
            if (glGetError() == GL_NO_ERROR)
              return true;
          }
          *params = GLES2Util::GetPreferredGLReadPixelsType(
              GetBoundReadFrameBufferInternalFormat(),
              GetBoundReadFrameBufferTextureType());
        }
        return true;
      case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
        *num_written = 1;
        if (params) {
          *params = group_->max_fragment_uniform_vectors();
        }
        return true;
      case GL_MAX_VARYING_VECTORS:
        *num_written = 1;
        if (params) {
          *params = group_->max_varying_vectors();
        }
        return true;
      case GL_MAX_VERTEX_UNIFORM_VECTORS:
        *num_written = 1;
        if (params) {
          *params = group_->max_vertex_uniform_vectors();
        }
        return true;
      }
  }
  switch (pname) {
    case GL_MAX_VIEWPORT_DIMS:
      if (offscreen_target_frame_buffer_.get()) {
        *num_written = 2;
        if (params) {
          params[0] = renderbuffer_manager()->max_renderbuffer_size();
          params[1] = renderbuffer_manager()->max_renderbuffer_size();
        }
        return true;
      }
      return false;
    case GL_MAX_SAMPLES:
      *num_written = 1;
      if (params) {
        params[0] = renderbuffer_manager()->max_samples();
      }
      return true;
    case GL_MAX_RENDERBUFFER_SIZE:
      *num_written = 1;
      if (params) {
        params[0] = renderbuffer_manager()->max_renderbuffer_size();
      }
      return true;
    case GL_MAX_TEXTURE_SIZE:
      *num_written = 1;
      if (params) {
        params[0] = texture_manager()->MaxSizeForTarget(GL_TEXTURE_2D);
      }
      return true;
    case GL_MAX_CUBE_MAP_TEXTURE_SIZE:
      *num_written = 1;
      if (params) {
        params[0] = texture_manager()->MaxSizeForTarget(GL_TEXTURE_CUBE_MAP);
      }
      return true;
    case GL_MAX_COLOR_ATTACHMENTS_EXT:
      *num_written = 1;
      if (params) {
        params[0] = group_->max_color_attachments();
      }
      return true;
    case GL_MAX_DRAW_BUFFERS_ARB:
      *num_written = 1;
      if (params) {
        params[0] = group_->max_draw_buffers();
      }
      return true;
    case GL_ALPHA_BITS:
      *num_written = 1;
      if (params) {
        GLint v = 0;
        glGetIntegerv(GL_ALPHA_BITS, &v);
        params[0] = BoundFramebufferHasColorAttachmentWithAlpha(false) ? v : 0;
      }
      return true;
    case GL_DEPTH_BITS:
      *num_written = 1;
      if (params) {
        GLint v = 0;
        glGetIntegerv(GL_DEPTH_BITS, &v);
        params[0] = BoundFramebufferHasDepthAttachment() ? v : 0;
      }
      return true;
    case GL_STENCIL_BITS:
      *num_written = 1;
      if (params) {
        GLint v = 0;
        glGetIntegerv(GL_STENCIL_BITS, &v);
        params[0] = BoundFramebufferHasStencilAttachment() ? v : 0;
      }
      return true;
    case GL_COMPRESSED_TEXTURE_FORMATS:
      *num_written = validators_->compressed_texture_format.GetValues().size();
      if (params) {
        for (GLint ii = 0; ii < *num_written; ++ii) {
          params[ii] = validators_->compressed_texture_format.GetValues()[ii];
        }
      }
      return true;
    case GL_NUM_COMPRESSED_TEXTURE_FORMATS:
      *num_written = 1;
      if (params) {
        *params = validators_->compressed_texture_format.GetValues().size();
      }
      return true;
    case GL_NUM_SHADER_BINARY_FORMATS:
      *num_written = 1;
      if (params) {
        *params = validators_->shader_binary_format.GetValues().size();
      }
      return true;
    case GL_SHADER_BINARY_FORMATS:
      *num_written = validators_->shader_binary_format.GetValues().size();
      if (params) {
        for (GLint ii = 0; ii <  *num_written; ++ii) {
          params[ii] = validators_->shader_binary_format.GetValues()[ii];
        }
      }
      return true;
    case GL_SHADER_COMPILER:
      *num_written = 1;
      if (params) {
        *params = GL_TRUE;
      }
      return true;
    case GL_ARRAY_BUFFER_BINDING:
      *num_written = 1;
      if (params) {
        if (state_.bound_array_buffer.get()) {
          GLuint client_id = 0;
          buffer_manager()->GetClientId(state_.bound_array_buffer->service_id(),
                                        &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_ELEMENT_ARRAY_BUFFER_BINDING:
      *num_written = 1;
      if (params) {
        if (state_.vertex_attrib_manager->element_array_buffer()) {
          GLuint client_id = 0;
          buffer_manager()->GetClientId(
              state_.vertex_attrib_manager->element_array_buffer()->
                  service_id(), &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_FRAMEBUFFER_BINDING:
      *num_written = 1;
      if (params) {
        Framebuffer* framebuffer =
            GetFramebufferInfoForTarget(GL_FRAMEBUFFER);
        if (framebuffer) {
          GLuint client_id = 0;
          framebuffer_manager()->GetClientId(
              framebuffer->service_id(), &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_READ_FRAMEBUFFER_BINDING_EXT:
      *num_written = 1;
      if (params) {
        Framebuffer* framebuffer =
            GetFramebufferInfoForTarget(GL_READ_FRAMEBUFFER_EXT);
        if (framebuffer) {
          GLuint client_id = 0;
          framebuffer_manager()->GetClientId(
              framebuffer->service_id(), &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_RENDERBUFFER_BINDING:
      *num_written = 1;
      if (params) {
        Renderbuffer* renderbuffer =
            GetRenderbufferInfoForTarget(GL_RENDERBUFFER);
        if (renderbuffer) {
          *params = renderbuffer->client_id();
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_CURRENT_PROGRAM:
      *num_written = 1;
      if (params) {
        if (state_.current_program.get()) {
          GLuint client_id = 0;
          program_manager()->GetClientId(
              state_.current_program->service_id(), &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_VERTEX_ARRAY_BINDING_OES:
      *num_written = 1;
      if (params) {
        if (state_.vertex_attrib_manager.get() !=
            state_.default_vertex_attrib_manager.get()) {
          GLuint client_id = 0;
          vertex_array_manager_->GetClientId(
              state_.vertex_attrib_manager->service_id(), &client_id);
          *params = client_id;
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_TEXTURE_BINDING_2D:
      *num_written = 1;
      if (params) {
        TextureUnit& unit = state_.texture_units[state_.active_texture_unit];
        if (unit.bound_texture_2d.get()) {
          *params = unit.bound_texture_2d->client_id();
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_TEXTURE_BINDING_CUBE_MAP:
      *num_written = 1;
      if (params) {
        TextureUnit& unit = state_.texture_units[state_.active_texture_unit];
        if (unit.bound_texture_cube_map.get()) {
          *params = unit.bound_texture_cube_map->client_id();
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_TEXTURE_BINDING_EXTERNAL_OES:
      *num_written = 1;
      if (params) {
        TextureUnit& unit = state_.texture_units[state_.active_texture_unit];
        if (unit.bound_texture_external_oes.get()) {
          *params = unit.bound_texture_external_oes->client_id();
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_TEXTURE_BINDING_RECTANGLE_ARB:
      *num_written = 1;
      if (params) {
        TextureUnit& unit = state_.texture_units[state_.active_texture_unit];
        if (unit.bound_texture_rectangle_arb.get()) {
          *params = unit.bound_texture_rectangle_arb->client_id();
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_UNPACK_FLIP_Y_CHROMIUM:
      *num_written = 1;
      if (params) {
        params[0] = unpack_flip_y_;
      }
      return true;
    case GL_UNPACK_PREMULTIPLY_ALPHA_CHROMIUM:
      *num_written = 1;
      if (params) {
        params[0] = unpack_premultiply_alpha_;
      }
      return true;
    case GL_UNPACK_UNPREMULTIPLY_ALPHA_CHROMIUM:
      *num_written = 1;
      if (params) {
        params[0] = unpack_unpremultiply_alpha_;
      }
      return true;
    case GL_BIND_GENERATES_RESOURCE_CHROMIUM:
      *num_written = 1;
      if (params) {
        params[0] = group_->bind_generates_resource() ? 1 : 0;
      }
      return true;
    default:
      if (pname >= GL_DRAW_BUFFER0_ARB &&
          pname < GL_DRAW_BUFFER0_ARB + group_->max_draw_buffers()) {
        *num_written = 1;
        if (params) {
          Framebuffer* framebuffer =
              GetFramebufferInfoForTarget(GL_FRAMEBUFFER);
          if (framebuffer) {
            params[0] = framebuffer->GetDrawBuffer(pname);
          } else {   
            if (pname == GL_DRAW_BUFFER0_ARB)
              params[0] = group_->draw_buffer();
            else
              params[0] = GL_NONE;
          }
        }
        return true;
      }
      *num_written = util_.GLGetNumValuesReturned(pname);
      return false;
  }
}
