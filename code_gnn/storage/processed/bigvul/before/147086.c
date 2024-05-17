void WebLocalFrameImpl::SetIsolatedWorldHumanReadableName(
    int world_id,
    const WebString& human_readable_name) {
  DCHECK(GetFrame());
  DOMWrapperWorld::SetNonMainWorldHumanReadableName(world_id,
                                                    human_readable_name);
}
