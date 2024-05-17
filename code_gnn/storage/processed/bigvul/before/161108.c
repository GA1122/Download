void CreateJpegDecodeAcceleratorOnIOThread(
    media::mojom::JpegDecodeAcceleratorRequest request) {
  auto* host =
      GpuProcessHost::Get(GpuProcessHost::GPU_PROCESS_KIND_SANDBOXED, false);
  if (host) {
    host->gpu_service()->CreateJpegDecodeAccelerator(std::move(request));
  } else {
    LOG(ERROR) << "No GpuProcessHost";
  }
}
