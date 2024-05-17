void MaybeRecordCTPolicyComplianceUseCounter(
    LocalFrame* frame,
    Resource::Type resource_type,
    ResourceResponse::CTPolicyCompliance compliance,
    DocumentLoader* loader) {
  if (compliance != ResourceResponse::kCTPolicyDoesNotComply)
    return;
  if (!frame->Tree().Parent() && resource_type == Resource::kMainResource)
    return;
  if (loader) {
    loader->GetUseCounter().Count(
        frame->Tree().Parent()
            ? WebFeature::kCertificateTransparencyNonCompliantResourceInSubframe
            : WebFeature::
                  kCertificateTransparencyNonCompliantSubresourceInMainFrame,
        frame);
  }
}
