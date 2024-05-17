  TestMetricsLog(const std::string& client_id,
                 int session_id,
                 LogType log_type,
                 MetricsServiceClient* client)
      : MetricsLog(client_id, session_id, log_type, client) {}
