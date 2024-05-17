ChildURLLoaderFactoryBundle* RenderFrameImpl::GetLoaderFactoryBundle() {
  if (!loader_factories_) {
    RenderFrameImpl* creator = RenderFrameImpl::FromWebFrame(
        frame_->Parent() ? frame_->Parent() : frame_->Opener());
    if (creator) {
      auto bundle_info =
          base::WrapUnique(static_cast<ChildURLLoaderFactoryBundleInfo*>(
              creator->GetLoaderFactoryBundle()->Clone().release()));
      loader_factories_ = base::MakeRefCounted<ChildURLLoaderFactoryBundle>(
          std::move(bundle_info));
    } else {
      SetupLoaderFactoryBundle(nullptr);
    }
  }
  return loader_factories_.get();
}
