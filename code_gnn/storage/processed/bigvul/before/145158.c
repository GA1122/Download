std::string GpuProcessHost::GetShaderPrefixKey() {
  if (shader_prefix_key_.empty()) {
    const gpu::GPUInfo info = GpuDataManagerImpl::GetInstance()->GetGPUInfo();
    const gpu::GPUInfo::GPUDevice& active_gpu = info.active_gpu();

    shader_prefix_key_ = GetContentClient()->GetProduct() + "-" +
                         info.gl_vendor + "-" + info.gl_renderer + "-" +
                         active_gpu.driver_version + "-" +
                         active_gpu.driver_vendor;

#if defined(OS_ANDROID)
    std::string build_fp =
        base::android::BuildInfo::GetInstance()->android_build_fp();
    CHECK(!build_fp.empty());
    shader_prefix_key_ += "-" + build_fp;
#endif
  }

  return shader_prefix_key_;
}
