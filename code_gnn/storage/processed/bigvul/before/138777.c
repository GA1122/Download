void RenderFrameHostImpl::OnUpdateToUniqueOrigin(
    bool is_potentially_trustworthy_unique_origin) {
  url::Origin origin;
  DCHECK(origin.unique());
  frame_tree_node()->SetCurrentOrigin(origin,
                                      is_potentially_trustworthy_unique_origin);
}
