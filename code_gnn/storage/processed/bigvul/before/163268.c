void InitDefaultJob() {
  base::ScopedZxHandle handle;
  zx_status_t result = zx_job_create(zx_job_default(), 0, handle.receive());
  CHECK_EQ(ZX_OK, result) << "zx_job_create(job): "
                          << zx_status_get_string(result);
  base::SetDefaultJob(std::move(handle));
}
