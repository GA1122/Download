void RenderFrameHostImpl::SetOriginOfNewFrame(
    const url::Origin& new_frame_creator) {
  DCHECK(!has_committed_any_navigation_);
  DCHECK(GetLastCommittedOrigin().opaque());

  bool new_frame_should_be_sandboxed =
      blink::WebSandboxFlags::kOrigin ==
      (frame_tree_node()->active_sandbox_flags() &
       blink::WebSandboxFlags::kOrigin);
  url::Origin new_frame_origin = new_frame_should_be_sandboxed
                                     ? new_frame_creator.DeriveNewOpaqueOrigin()
                                     : new_frame_creator;
  SetLastCommittedOrigin(new_frame_origin);
}
