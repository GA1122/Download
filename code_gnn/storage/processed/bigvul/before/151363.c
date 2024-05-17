void RecordTiming(const ResourceLoadTiming& timing, TracedValue* value) {
  value->SetDouble("requestTime", timing.RequestTime());
  value->SetDouble("proxyStart",
                   timing.CalculateMillisecondDelta(timing.ProxyStart()));
  value->SetDouble("proxyEnd",
                   timing.CalculateMillisecondDelta(timing.ProxyEnd()));
  value->SetDouble("dnsStart",
                   timing.CalculateMillisecondDelta(timing.DnsStart()));
  value->SetDouble("dnsEnd", timing.CalculateMillisecondDelta(timing.DnsEnd()));
  value->SetDouble("connectStart",
                   timing.CalculateMillisecondDelta(timing.ConnectStart()));
  value->SetDouble("connectEnd",
                   timing.CalculateMillisecondDelta(timing.ConnectEnd()));
  value->SetDouble("sslStart",
                   timing.CalculateMillisecondDelta(timing.SslStart()));
  value->SetDouble("sslEnd", timing.CalculateMillisecondDelta(timing.SslEnd()));
  value->SetDouble("workerStart",
                   timing.CalculateMillisecondDelta(timing.WorkerStart()));
  value->SetDouble("workerReady",
                   timing.CalculateMillisecondDelta(timing.WorkerReady()));
  value->SetDouble("sendStart",
                   timing.CalculateMillisecondDelta(timing.SendStart()));
  value->SetDouble("sendEnd",
                   timing.CalculateMillisecondDelta(timing.SendEnd()));
  value->SetDouble("receiveHeadersEnd", timing.CalculateMillisecondDelta(
                                            timing.ReceiveHeadersEnd()));
  value->SetDouble("pushStart", timing.PushStart());
  value->SetDouble("pushEnd", timing.PushEnd());
}
