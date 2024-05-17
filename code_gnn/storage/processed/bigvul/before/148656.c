void SkiaOutputSurfaceImpl::PrepareYUVATextureIndices(
    const std::vector<ImageContext*>& contexts,
    bool has_alpha,
    SkYUVAIndex indices[4]) {
  DCHECK((has_alpha && (contexts.size() == 3 || contexts.size() == 4)) ||
         (!has_alpha && (contexts.size() == 2 || contexts.size() == 3)));

  bool uv_interleaved = has_alpha ? contexts.size() == 3 : contexts.size() == 2;

  indices[SkYUVAIndex::kY_Index].fIndex = 0;
  indices[SkYUVAIndex::kY_Index].fChannel = SkColorChannel::kR;

  if (uv_interleaved) {
    indices[SkYUVAIndex::kU_Index].fIndex = 1;
    indices[SkYUVAIndex::kU_Index].fChannel = SkColorChannel::kR;

    indices[SkYUVAIndex::kV_Index].fIndex = 1;
    indices[SkYUVAIndex::kV_Index].fChannel = SkColorChannel::kG;

    indices[SkYUVAIndex::kA_Index].fIndex = has_alpha ? 2 : -1;
    indices[SkYUVAIndex::kA_Index].fChannel = SkColorChannel::kR;
  } else {
    indices[SkYUVAIndex::kU_Index].fIndex = 1;
    indices[SkYUVAIndex::kU_Index].fChannel = SkColorChannel::kR;

    indices[SkYUVAIndex::kV_Index].fIndex = 2;
    indices[SkYUVAIndex::kV_Index].fChannel = SkColorChannel::kR;

    indices[SkYUVAIndex::kA_Index].fIndex = has_alpha ? 3 : -1;
    indices[SkYUVAIndex::kA_Index].fChannel = SkColorChannel::kR;
  }
}
