NPPFindExtensions* WebPluginDelegatePepper::GetFindExtensions() {
  NPPFindExtensions* ret = NULL;
  NPPExtensions* extensions = NULL;
  instance()->NPP_GetValue(NPPVPepperExtensions, &extensions);
  if (extensions && extensions->getFindExtensions)
    ret = extensions->getFindExtensions(instance()->npp());
  return ret;
}
