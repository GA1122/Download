    TextureManager::TextureInfo::Ref GetInfoForSamplerType(GLenum type) {
      DCHECK(type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE ||
             type == GL_SAMPLER_EXTERNAL_OES);
      return type == GL_SAMPLER_2D ? bound_texture_2d :
          (type == GL_SAMPLER_EXTERNAL_OES ? bound_texture_external_oes :
              bound_texture_cube_map);
    }
