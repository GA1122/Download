void RenderFrameImpl::SetupLoaderFactoryBundle(
    std::unique_ptr<URLLoaderFactoryBundleInfo> info) {
  RenderThreadImpl* render_thread = RenderThreadImpl::current();

  if (render_thread) {
    loader_factories_ = render_thread->blink_platform_impl()
                            ->CreateDefaultURLLoaderFactoryBundle();
  } else {
    loader_factories_ = base::MakeRefCounted<ChildURLLoaderFactoryBundle>();
  }

  if (info) {
    static_cast<URLLoaderFactoryBundle*>(loader_factories_.get())
        ->Update(std::move(info));
  }
}
