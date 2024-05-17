base::SupportsUserData::Data* DataUseUserData::Create(
    ServiceName service_name) {
  return new DataUseUserData(service_name, GetCurrentAppState());
}
