static ImageLoader::BypassMainWorldBehavior ShouldBypassMainWorldCSP(
    ImageLoader* loader) {
  DCHECK(loader);
  DCHECK(loader->GetElement());
  if (loader->GetElement()->GetDocument().GetFrame() &&
      loader->GetElement()
          ->GetDocument()
          .GetFrame()
          ->GetScriptController()
          .ShouldBypassMainWorldCSP())
    return ImageLoader::kBypassMainWorldCSP;
  return ImageLoader::kDoNotBypassMainWorldCSP;
}
