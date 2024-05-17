bool WebLocalFrameImpl::HasSelection() const {
  DCHECK(GetFrame());
  WebPluginContainerImpl* plugin_container =
      GetFrame()->GetWebPluginContainer();
  if (plugin_container)
    return plugin_container->Plugin()->HasSelection();

  return GetFrame()
             ->Selection()
             .ComputeVisibleSelectionInDOMTreeDeprecated()
             .Start() != GetFrame()
                             ->Selection()
                             .ComputeVisibleSelectionInDOMTreeDeprecated()
                             .End();
}
