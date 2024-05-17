void FrameFetchContext::CountUsage(WebFeature feature) const {
  if (IsDetached())
    return;
  if (DocumentLoader* loader = MasterDocumentLoader())
    loader->GetUseCounter().Count(feature, GetFrame());
}
