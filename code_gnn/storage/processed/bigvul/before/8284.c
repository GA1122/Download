XRRScreenSize *XRRConfigSizes(XRRScreenConfiguration *config, int *nsizes)
{
   *nsizes = config->nsizes;
  return config->sizes;
}
