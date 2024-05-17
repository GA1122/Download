GpuTextureResultR16_L16 GpuTextureUMAHelper() {
  if (g_r16_is_present && g_l16_is_present) {
    return GpuTextureResultR16_L16::kHaveR16AndL16;
  } else if (g_r16_is_present) {
    return GpuTextureResultR16_L16::kHaveR16;
  } else if (g_l16_is_present) {
    return GpuTextureResultR16_L16::kHaveL16;
  }
  return GpuTextureResultR16_L16::kHaveNone;
}
