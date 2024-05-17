static ImageLoader::BypassMainWorldBehavior shouldBypassMainWorldCSP(
    ImageLoader* loader) {
  DCHECK(loader);
  DCHECK(loader->element());
  if (loader->element()->document().frame() &&
      loader->element()
          ->document()
          .frame()
          ->script()
          .shouldBypassMainWorldCSP())
    return ImageLoader::BypassMainWorldCSP;
  return ImageLoader::DoNotBypassMainWorldCSP;
}
