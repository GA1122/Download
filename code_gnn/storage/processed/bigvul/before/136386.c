void PaintArtifactCompositor::EnableExtraDataForTesting() {
  extra_data_for_testing_enabled_ = true;
  extra_data_for_testing_ = std::make_unique<ExtraDataForTesting>();
}
