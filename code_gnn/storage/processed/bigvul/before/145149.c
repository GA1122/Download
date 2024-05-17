void GpuProcessHost::DisableGpuCompositing() {
#if !defined(OS_ANDROID)
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE, base::BindOnce([]() {
        if (auto* factory = ImageTransportFactory::GetInstance())
          factory->DisableGpuCompositing();
      }));
#endif
}
