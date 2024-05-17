bool GLES2DecoderImpl::GetHelper(
    GLenum pname, GLint* params, GLsizei* num_written) {
  DCHECK(num_written);
  switch (pname) {
    case GL_IMPLEMENTATION_COLOR_READ_FORMAT:
    case GL_IMPLEMENTATION_COLOR_READ_TYPE:
      *num_written = 1;
      {
        Framebuffer* framebuffer = GetBoundReadFramebuffer();
        if (framebuffer &&
            framebuffer->IsPossiblyComplete(feature_info_.get()) !=
            GL_FRAMEBUFFER_COMPLETE) {
          LOCAL_SET_GL_ERROR(
              GL_INVALID_OPERATION, "glGetIntegerv", "incomplete framebuffer");
          if (params) {
            *params = 0;
          }
          return true;
        }
      }
      if (params) {
        if (feature_info_->gl_version_info().is_es) {
          api()->glGetIntegervFn(pname, params);
        } else {
          if (pname == GL_IMPLEMENTATION_COLOR_READ_FORMAT) {
            *params = GLES2Util::GetGLReadPixelsImplementationFormat(
                GetBoundReadFramebufferInternalFormat(),
                GetBoundReadFramebufferTextureType(),
                feature_info_->feature_flags().ext_read_format_bgra);
          } else {
            *params = GLES2Util::GetGLReadPixelsImplementationType(
                GetBoundReadFramebufferInternalFormat(),
                GetBoundReadFramebufferTextureType());
          }
        }
        if (*params == GL_HALF_FLOAT && feature_info_->IsWebGL1OrES2Context()) {
          *params = GL_HALF_FLOAT_OES;
        }
        if (*params == GL_SRGB_ALPHA_EXT) {
          *params = GL_RGBA;
        }
        if (*params == GL_SRGB_EXT) {
          *params = GL_RGB;
        }
      }
      return true;
    default:
      break;
  }

  if (!gl_version_info().is_es) {
    switch (pname) {
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
  if (feature_info_->IsWebGL2OrES3Context()) {
    switch (pname) {
      case GL_MAX_VARYING_COMPONENTS: {
        if (gl_version_info().is_es) {
          *num_written = 1;
          break;
        }

        GLint max_varying_vectors = 0;
        api()->glGetIntegervFn(GL_MAX_VARYING_VECTORS, &max_varying_vectors);
        *num_written = 1;
        if (params) {
          *params = max_varying_vectors * 4;
        }
        return true;
      }
      case GL_READ_BUFFER:
        *num_written = 1;
        if (params) {
          Framebuffer* framebuffer = GetBoundReadFramebuffer();
          GLenum read_buffer;
          if (framebuffer) {
            read_buffer = framebuffer->read_buffer();
          } else {
            read_buffer = back_buffer_read_buffer_;
          }
          *params = static_cast<GLint>(read_buffer);
        }
        return true;
      case GL_TRANSFORM_FEEDBACK_ACTIVE:
        *num_written = 1;
        if (params) {
          *params =
              static_cast<GLint>(state_.bound_transform_feedback->active());
        }
        return true;
      case GL_TRANSFORM_FEEDBACK_PAUSED:
        *num_written = 1;
        if (params) {
          *params =
              static_cast<GLint>(state_.bound_transform_feedback->paused());
        }
        return true;
      case GL_WINDOW_RECTANGLE_EXT:
        *num_written = 4;
        DCHECK(!params);
        return true;
    }
  }
  switch (pname) {
    case GL_MAX_VIEWPORT_DIMS:
      *num_written = 2;
      if (offscreen_target_frame_buffer_.get()) {
        if (params) {
          params[0] = renderbuffer_manager()->max_renderbuffer_size();
          params[1] = renderbuffer_manager()->max_renderbuffer_size();
        }
        return true;
      }
      break;
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
    case GL_MAX_3D_TEXTURE_SIZE:
      *num_written = 1;
      if (params) {
        params[0] = texture_manager()->MaxSizeForTarget(GL_TEXTURE_3D);
      }
      return true;
    case GL_MAX_ARRAY_TEXTURE_LAYERS:
      *num_written = 1;
      if (params) {
        params[0] = texture_manager()->max_array_texture_layers();
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
        Framebuffer* framebuffer = GetBoundDrawFramebuffer();
        if (framebuffer) {
          if (framebuffer->HasAlphaMRT() &&
              framebuffer->HasSameInternalFormatsMRT()) {
            if (gl_version_info().is_desktop_core_profile) {
              for (uint32_t i = 0; i < group_->max_draw_buffers(); i++) {
                if (framebuffer->HasColorAttachment(i)) {
                  api()->glGetFramebufferAttachmentParameterivEXTFn(
                      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                      GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &v);
                  break;
                }
              }
            } else {
              api()->glGetIntegervFn(GL_ALPHA_BITS, &v);
            }
          }
        } else {
          v = (ClientExposedBackBufferHasAlpha() ? 8 : 0);
        }
        params[0] = v;
      }
      return true;
    case GL_DEPTH_BITS:
      *num_written = 1;
      if (params) {
        GLint v = 0;
        if (gl_version_info().is_desktop_core_profile) {
          Framebuffer* framebuffer = GetBoundDrawFramebuffer();
          if (framebuffer) {
            if (framebuffer->HasDepthAttachment()) {
              api()->glGetFramebufferAttachmentParameterivEXTFn(
                  GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                  GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &v);
            }
          } else {
            v = (back_buffer_has_depth_ ? 24 : 0);
          }
        } else {
          api()->glGetIntegervFn(GL_DEPTH_BITS, &v);
        }
        params[0] = BoundFramebufferHasDepthAttachment() ? v : 0;
      }
      return true;
    case GL_RED_BITS:
    case GL_GREEN_BITS:
    case GL_BLUE_BITS:
      *num_written = 1;
      if (params) {
        GLint v = 0;
        if (gl_version_info().is_desktop_core_profile) {
          Framebuffer* framebuffer = GetBoundDrawFramebuffer();
          if (framebuffer) {
            if (framebuffer->HasSameInternalFormatsMRT()) {
              GLenum framebuffer_enum = 0;
              switch (pname) {
                case GL_RED_BITS:
                  framebuffer_enum = GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE;
                  break;
                case GL_GREEN_BITS:
                  framebuffer_enum = GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE;
                  break;
                case GL_BLUE_BITS:
                  framebuffer_enum = GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE;
                  break;
              }
              for (uint32_t i = 0; i < group_->max_draw_buffers(); i++) {
                if (framebuffer->HasColorAttachment(i)) {
                  api()->glGetFramebufferAttachmentParameterivEXTFn(
                      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                      framebuffer_enum, &v);
                  break;
                }
              }
            }
          } else {
            v = 8;
          }
        } else {
          api()->glGetIntegervFn(pname, &v);
        }
        params[0] = v;
      }
      return true;
    case GL_STENCIL_BITS:
      *num_written = 1;
      if (params) {
        GLint v = 0;
        if (gl_version_info().is_desktop_core_profile) {
          Framebuffer* framebuffer = GetBoundDrawFramebuffer();
          if (framebuffer) {
            if (framebuffer->HasStencilAttachment()) {
              api()->glGetFramebufferAttachmentParameterivEXTFn(
                  GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
                  GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &v);
            }
          } else {
            v = (back_buffer_has_stencil_ ? 8 : 0);
          }
        } else {
          api()->glGetIntegervFn(GL_STENCIL_BITS, &v);
        }
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
        *params = GetClientId(
            buffer_manager(), state_.bound_array_buffer.get());
      }
      return true;
    case GL_ELEMENT_ARRAY_BUFFER_BINDING:
      *num_written = 1;
      if (params) {
        *params = GetClientId(
            buffer_manager(),
            state_.vertex_attrib_manager->element_array_buffer());
      }
      return true;
    case GL_COPY_READ_BUFFER_BINDING:
      *num_written = 1;
      if (params) {
        *params = GetClientId(
            buffer_manager(), state_.bound_copy_read_buffer.get());
      }
      return true;
    case GL_COPY_WRITE_BUFFER_BINDING:
      *num_written = 1;
      if (params) {
        *params = GetClientId(
            buffer_manager(), state_.bound_copy_write_buffer.get());
      }
      return true;
    case GL_PIXEL_PACK_BUFFER_BINDING:
      *num_written = 1;
      if (params) {
        *params = GetClientId(
            buffer_manager(), state_.bound_pixel_pack_buffer.get());
      }
      return true;
    case GL_PIXEL_UNPACK_BUFFER_BINDING:
      *num_written = 1;
      if (params) {
        *params = GetClientId(
            buffer_manager(), state_.bound_pixel_unpack_buffer.get());
      }
      return true;
    case GL_TRANSFORM_FEEDBACK_BUFFER_BINDING:
      *num_written = 1;
      if (params) {
        *params = GetClientId(
            buffer_manager(), state_.bound_transform_feedback_buffer.get());
      }
      return true;
    case GL_UNIFORM_BUFFER_BINDING:
      *num_written = 1;
      if (params) {
        *params = GetClientId(
            buffer_manager(), state_.bound_uniform_buffer.get());
      }
      return true;
    case GL_FRAMEBUFFER_BINDING:
      *num_written = 1;
      if (params) {
        *params = GetClientId(
            framebuffer_manager(),
            GetFramebufferInfoForTarget(GL_FRAMEBUFFER));
      }
      return true;
    case GL_READ_FRAMEBUFFER_BINDING_EXT:
      *num_written = 1;
      if (params) {
        *params = GetClientId(
            framebuffer_manager(),
            GetFramebufferInfoForTarget(GL_READ_FRAMEBUFFER_EXT));
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
        *params = GetClientId(program_manager(), state_.current_program.get());
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
    case GL_BIND_GENERATES_RESOURCE_CHROMIUM:
      *num_written = 1;
      if (params) {
        params[0] = group_->bind_generates_resource() ? 1 : 0;
      }
      return true;
    case GL_MAX_DUAL_SOURCE_DRAW_BUFFERS_EXT:
      *num_written = 1;
      if (params) {
        params[0] = group_->max_dual_source_draw_buffers();
      }
      return true;

    case GL_MAJOR_VERSION:
      *num_written = 1;
      if (params) {
        params[0] = 3;
      }
      return true;
    case GL_MINOR_VERSION:
      *num_written = 1;
      if (params) {
        params[0] = 0;
      }
      return true;

    case GL_NUM_EXTENSIONS:
      *num_written = 1;
      if (params) {
        params[0] = 0;
      }
      return true;
    case GL_GPU_DISJOINT_EXT:
      *num_written = 1;
      if (params) {
        params[0] = 0;
      }
      return true;
    case GL_TIMESTAMP_EXT:
      *num_written = 1;
      if (params) {
        params[0] = 0;
      }
      return true;
    case GL_TEXTURE_BINDING_2D_ARRAY:
      *num_written = 1;
      if (params) {
        TextureUnit& unit = state_.texture_units[state_.active_texture_unit];
        if (unit.bound_texture_2d_array.get()) {
          *params = unit.bound_texture_2d_array->client_id();
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_TEXTURE_BINDING_3D:
      *num_written = 1;
      if (params) {
        TextureUnit& unit = state_.texture_units[state_.active_texture_unit];
        if (unit.bound_texture_3d.get()) {
          *params = unit.bound_texture_3d->client_id();
        } else {
          *params = 0;
        }
      }
      return true;
    case GL_SAMPLER_BINDING:
      *num_written = 1;
      if (params) {
        DCHECK_LT(state_.active_texture_unit, state_.sampler_units.size());
        Sampler* sampler =
            state_.sampler_units[state_.active_texture_unit].get();
        *params = sampler ? sampler->client_id() : 0;
      }
      return true;
    case GL_TRANSFORM_FEEDBACK_BINDING:
      *num_written = 1;
      if (params) {
        *params = state_.bound_transform_feedback->client_id();
      }
      return true;
    case GL_NUM_PROGRAM_BINARY_FORMATS:
      *num_written = 1;
      if (params) {
        *params = 0;
      }
      return true;
    case GL_PROGRAM_BINARY_FORMATS:
      *num_written = 0;
      return true;
    default:
      if (pname >= GL_DRAW_BUFFER0_ARB && pname <= GL_DRAW_BUFFER15_ARB) {
        *num_written = 1;
        if (params) {
          if (pname < GL_DRAW_BUFFER0_ARB + group_->max_draw_buffers()) {
            Framebuffer* framebuffer =
                GetFramebufferInfoForTarget(GL_FRAMEBUFFER);
            if (framebuffer) {
              *params = framebuffer->GetDrawBuffer(pname);
            } else {   
              if (pname == GL_DRAW_BUFFER0_ARB)
                *params = back_buffer_draw_buffer_;
              else
                *params = GL_NONE;
            }
          } else {
            *params = GL_NONE;
          }
        }
        return true;
      }

      *num_written = util_.GLGetNumValuesReturned(pname);
      if (*num_written)
        break;

      return false;
  }

  if (params) {
    DCHECK(*num_written);
    pname = AdjustGetPname(pname);
    api()->glGetIntegervFn(pname, params);
  }
  return true;
}
