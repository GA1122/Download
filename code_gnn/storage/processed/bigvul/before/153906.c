bool GLES2DecoderImpl::ClearLevel3D(Texture* texture,
                                    unsigned target,
                                    int level,
                                    unsigned format,
                                    unsigned type,
                                    int width,
                                    int height,
                                    int depth) {
  DCHECK(target == GL_TEXTURE_3D || target == GL_TEXTURE_2D_ARRAY);
  DCHECK(feature_info_->IsWebGL2OrES3Context());
  if (width == 0 || height == 0 || depth == 0)
    return true;

  uint32_t size;
  uint32_t padded_row_size;
  uint32_t padding;
  PixelStoreParams params;
  params.alignment = state_.unpack_alignment;
  if (!GLES2Util::ComputeImageDataSizesES3(width, height, depth,
                                           format, type,
                                           params,
                                           &size,
                                           nullptr,
                                           &padded_row_size,
                                           nullptr,
                                           &padding)) {
    return false;
  }
  const uint32_t kMaxZeroSize = 1024 * 1024 * 2;
  uint32_t buffer_size;
  std::vector<TexSubCoord3D> subs;
  if (size < kMaxZeroSize) {
    buffer_size = size;
    subs.push_back(TexSubCoord3D(0, 0, 0, width, height, depth));
  } else {
    uint32_t size_per_layer;
    if (!base::CheckMul(padded_row_size, height)
             .AssignIfValid(&size_per_layer)) {
      return false;
    }
    if (size_per_layer < kMaxZeroSize) {
      uint32_t depth_step = kMaxZeroSize / size_per_layer;
      uint32_t num_of_slices = depth / depth_step;
      if (num_of_slices * depth_step < static_cast<uint32_t>(depth))
        num_of_slices++;
      DCHECK_LT(0u, num_of_slices);
      buffer_size = size_per_layer * depth_step;
      int depth_of_last_slice = depth - (num_of_slices - 1) * depth_step;
      DCHECK_LT(0, depth_of_last_slice);
      for (uint32_t ii = 0; ii < num_of_slices; ++ii) {
        int depth_ii =
            (ii + 1 == num_of_slices ? depth_of_last_slice : depth_step);
        subs.push_back(
            TexSubCoord3D(0, 0, depth_step * ii, width, height, depth_ii));
      }
    } else {
      if (kMaxZeroSize < padded_row_size) {
        return false;
      }
      uint32_t height_step = kMaxZeroSize / padded_row_size;
      uint32_t num_of_slices = height / height_step;
      if (num_of_slices * height_step < static_cast<uint32_t>(height))
        num_of_slices++;
      DCHECK_LT(0u, num_of_slices);
      buffer_size = padded_row_size * height_step;
      int height_of_last_slice = height - (num_of_slices - 1) * height_step;
      DCHECK_LT(0, height_of_last_slice);
      for (int zz = 0; zz < depth; ++zz) {
        for (uint32_t ii = 0; ii < num_of_slices; ++ii) {
          int height_ii =
              (ii + 1 == num_of_slices ? height_of_last_slice : height_step);
          subs.push_back(
              TexSubCoord3D(0, height_step * ii, zz, width, height_ii, 1));
        }
      }
    }
  }

  TRACE_EVENT1("gpu", "GLES2DecoderImpl::ClearLevel3D", "size", size);

  {
    ScopedPixelUnpackState reset_restore(&state_);
    GLuint buffer_id = 0;
    api()->glGenBuffersARBFn(1, &buffer_id);
    api()->glBindBufferFn(GL_PIXEL_UNPACK_BUFFER, buffer_id);
    {
      buffer_size += padding;
      std::unique_ptr<char[]> zero(new char[buffer_size]);
      memset(zero.get(), 0, buffer_size);
      api()->glBufferDataFn(GL_PIXEL_UNPACK_BUFFER, buffer_size, zero.get(),
                            GL_STATIC_DRAW);
    }

    api()->glBindTextureFn(texture->target(), texture->service_id());
    for (size_t ii = 0; ii < subs.size(); ++ii) {
      api()->glTexSubImage3DFn(target, level, subs[ii].xoffset,
                               subs[ii].yoffset, subs[ii].zoffset,
                               subs[ii].width, subs[ii].height, subs[ii].depth,
                               format, type, nullptr);
    }
    api()->glDeleteBuffersARBFn(1, &buffer_id);
  }

  TextureRef* bound_texture =
      texture_manager()->GetTextureInfoForTarget(&state_, texture->target());
  api()->glBindTextureFn(texture->target(),
                         bound_texture ? bound_texture->service_id() : 0);
  return true;
}
