  GLenum GetBindTargetForSamplerType(GLenum type) {
    DCHECK(type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE ||
           type == GL_SAMPLER_EXTERNAL_OES);
    return type == GL_SAMPLER_2D ? GL_TEXTURE_2D :
        (type == GL_SAMPLER_EXTERNAL_OES ? GL_TEXTURE_EXTERNAL_OES :
            GL_TEXTURE_CUBE_MAP);
  }
