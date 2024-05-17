void RecordLegacySymantecCertUseCounter(LocalFrame* frame,
                                        Resource::Type resource_type) {
  if (resource_type == Resource::kMainResource) {
    return;
  }
  UseCounter::Count(frame, WebFeature::kLegacySymantecCertInSubresource);
}
