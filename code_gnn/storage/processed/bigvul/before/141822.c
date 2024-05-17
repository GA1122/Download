MetricsLog::MetricsLog(const std::string& client_id,
                       int session_id,
                       LogType log_type,
                       MetricsServiceClient* client)
    : closed_(false),
      log_type_(log_type),
      client_(client),
      creation_time_(base::TimeTicks::Now()),
      has_environment_(false) {
  if (IsTestingID(client_id))
    uma_proto_.set_client_id(0);
  else
    uma_proto_.set_client_id(Hash(client_id));

  uma_proto_.set_session_id(session_id);

  const int32_t product = client_->GetProduct();
  if (product != uma_proto_.product())
    uma_proto_.set_product(product);

  SystemProfileProto* system_profile = uma_proto()->mutable_system_profile();
  RecordCoreSystemProfile(client_, system_profile);
}
