void GLES2DecoderImpl::SetLevelInfo(uint32_t client_id,
                                    int level,
                                    unsigned internal_format,
                                    unsigned width,
                                    unsigned height,
                                    unsigned depth,
                                    unsigned format,
                                    unsigned type,
                                    const gfx::Rect& cleared_rect) {
  TextureRef* texture_ref = texture_manager()->GetTexture(client_id);
  texture_manager()->SetLevelInfo(texture_ref, texture_ref->texture()->target(),
                                  level, internal_format, width, height, depth,
                                  0  , format, type, cleared_rect);
}
