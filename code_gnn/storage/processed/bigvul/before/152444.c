void RenderFrameImpl::OnReload(bool bypass_cache) {
  frame_->StartReload(bypass_cache ? WebFrameLoadType::kReloadBypassingCache
                                   : WebFrameLoadType::kReload);
}
