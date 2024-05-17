    TextureManager::TextureInfo::Ref GetInfoForSamplerType(GLenum type) {
      DCHECK(type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE ||
             type == GL_SAMPLER_EXTERNAL_OES || type == GL_SAMPLER_2D_RECT_ARB);
      switch (type) {
        case GL_SAMPLER_2D:
          return bound_texture_2d;
        case GL_SAMPLER_CUBE:
          return bound_texture_cube_map;
        case GL_SAMPLER_EXTERNAL_OES:
          return bound_texture_external_oes;
        case GL_SAMPLER_2D_RECT_ARB:
          return bound_texture_rectangle_arb;
      }

      NOTREACHED();
      return NULL;
    }
