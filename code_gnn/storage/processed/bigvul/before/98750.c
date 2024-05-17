void WebPluginDelegatePepper::Zoom(int factor) {
  NPPExtensions* extensions = NULL;
  instance()->NPP_GetValue(NPPVPepperExtensions, &extensions);
  if (extensions && extensions->zoom)
    extensions->zoom(instance()->npp(), factor);
}
