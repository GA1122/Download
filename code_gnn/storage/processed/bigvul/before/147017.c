bool WebLocalFrameImpl::IsPrintScalingDisabledForPlugin(const WebNode& node) {
  DCHECK(GetFrame());
  WebPluginContainerImpl* plugin_container =
      node.IsNull() ? GetFrame()->GetWebPluginContainer()
                    : ToWebPluginContainerImpl(node.PluginContainer());

  if (!plugin_container || !plugin_container->SupportsPaginatedPrint())
    return false;

  return plugin_container->IsPrintScalingDisabled();
}
