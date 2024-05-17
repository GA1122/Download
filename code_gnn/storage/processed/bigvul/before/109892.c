void AcceleratedSurfaceBuffersSwappedCompleted(int host_id,
                                               int route_id,
                                               bool alive) {
  if (BrowserThread::CurrentlyOn(BrowserThread::IO)) {
    GpuProcessHost* host = GpuProcessHost::FromID(host_id);
    if (host) {
      if (alive)
        host->Send(new AcceleratedSurfaceMsg_BuffersSwappedACK(route_id));
      else {
        host->ForceShutdown();
      }
    }
  } else {
    BrowserThread::PostTask(
      BrowserThread::IO,
      FROM_HERE,
      base::Bind(&AcceleratedSurfaceBuffersSwappedCompleted,
                 host_id,
                 route_id,
                 alive));
  }
}
