GpuProcessHost::SurfaceRef::~SurfaceRef() {
  BrowserThread::PostTask(BrowserThread::UI,
                          FROM_HERE,
                          base::Bind(&ReleasePermanentXIDDispatcher, surface_));
}
