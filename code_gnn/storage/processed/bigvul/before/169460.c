NetworkChangeNotifierMac::NetworkChangeCalculatorParamsMac() {
  NetworkChangeCalculatorParams params;
  params.ip_address_offline_delay_ = base::TimeDelta::FromMilliseconds(500);
  params.ip_address_online_delay_ = base::TimeDelta::FromMilliseconds(500);
  params.connection_type_offline_delay_ =
      base::TimeDelta::FromMilliseconds(1000);
  params.connection_type_online_delay_ = base::TimeDelta::FromMilliseconds(500);
  return params;
}
