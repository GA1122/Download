viz::mojom::GpuService* GpuProcessHost::gpu_service() {
  DCHECK(gpu_service_ptr_.is_bound());
  return gpu_service_ptr_.get();
}
