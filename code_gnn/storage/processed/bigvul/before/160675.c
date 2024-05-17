void RenderFrameImpl::OnReload(bool bypass_cache) {
  frame_->Reload(bypass_cache ? WebFrameLoadType::kReloadBypassingCache
                              : WebFrameLoadType::kReload);
}
