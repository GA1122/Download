void RenderFrameHostImpl::AXContentNodeDataToAXNodeData(
    const AXContentNodeData& src,
    ui::AXNodeData* dst) {
  *dst = src;

  for (auto iter : src.content_int_attributes) {
    AXContentIntAttribute attr = iter.first;
    int32_t value = iter.second;
    switch (attr) {
      case AX_CONTENT_ATTR_CHILD_ROUTING_ID:
        dst->string_attributes.push_back(
            std::make_pair(ax::mojom::StringAttribute::kChildTreeId,
                           RoutingIDToAXTreeID(value).ToString()));
        break;
      case AX_CONTENT_ATTR_CHILD_BROWSER_PLUGIN_INSTANCE_ID:
        dst->string_attributes.push_back(std::make_pair(
            ax::mojom::StringAttribute::kChildTreeId,
            BrowserPluginInstanceIDToAXTreeID(value).ToString()));
        break;
      case AX_CONTENT_INT_ATTRIBUTE_LAST:
        NOTREACHED();
        break;
    }
  }
}
