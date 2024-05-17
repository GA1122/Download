void DataReductionProxyConfig::AddDefaultProxyBypassRules() {
  DCHECK(configurator_);
  configurator_->SetBypassRules(
      "<local>,"

      "ws://*,"
      "wss://*,"

      "0.0.0.0/8,"

      "10.0.0.0/8,"
      "172.16.0.0/12,"
      "192.168.0.0/16,"

      "::/128,"

      "fc00::/7,"

      "*-ds.metric.gstatic.com,"
      "*-v4.metric.gstatic.com");
}
