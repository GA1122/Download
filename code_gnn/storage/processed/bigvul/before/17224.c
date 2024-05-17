void BrowserMainParts::PreEarlyInitialization() {
  content::SetWebContentsViewOxideFactory(WebContentsView::Create);
#if defined(OS_LINUX)
  media::SetVideoCaptureDeviceFactoryOverrideDelegate(
      OverrideVideoCaptureDeviceFactory);
#endif
  ui::SetClipboardOxideFactory(CreateClipboard);

#if defined(OS_LINUX)
  gpu_info_collector_.reset(CreateGpuInfoCollectorLinux());
  gpu::SetGpuInfoCollectorOxideLinux(gpu_info_collector_.get());
#endif

  base::MessageLoop::InitMessagePumpForUIFactory(CreateUIMessagePump);
  main_message_loop_.reset(new base::MessageLoop(base::MessageLoop::TYPE_UI));
  base::MessageLoop::InitMessagePumpForUIFactory(nullptr);
}
