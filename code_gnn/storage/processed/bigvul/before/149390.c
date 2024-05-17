void AwFeatureListCreator::CreateFeatureListAndFieldTrials() {
  CreateLocalState();
  AwMetricsServiceClient::GetInstance()->Initialize(local_state_.get());
  SetUpFieldTrials();
}
