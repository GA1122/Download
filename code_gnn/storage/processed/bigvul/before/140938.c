SVGDocumentExtensions& Document::AccessSVGExtensions() {
  if (!svg_extensions_)
    svg_extensions_ = MakeGarbageCollected<SVGDocumentExtensions>(this);
  return *svg_extensions_;
}
