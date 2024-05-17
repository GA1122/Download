NPPPrintExtensions* WebPluginDelegatePepper::GetPrintExtensions() {
  NPPPrintExtensions* ret = NULL;
  NPPExtensions* extensions = NULL;
  instance()->NPP_GetValue(NPPVPepperExtensions, &extensions);
  if (extensions && extensions->getPrintExtensions)
    ret = extensions->getPrintExtensions(instance()->npp());
  return ret;
}
