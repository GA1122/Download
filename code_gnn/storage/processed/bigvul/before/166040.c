WebRTCLegacyStatsMemberTypeFromStatsValueType(
    webrtc::StatsReport::Value::Type type) {
  switch (type) {
    case StatsReport::Value::kInt:
      return blink::kWebRTCLegacyStatsMemberTypeInt;
    case StatsReport::Value::kInt64:
      return blink::kWebRTCLegacyStatsMemberTypeInt64;
    case StatsReport::Value::kFloat:
      return blink::kWebRTCLegacyStatsMemberTypeFloat;
    case StatsReport::Value::kString:
    case StatsReport::Value::kStaticString:
      return blink::kWebRTCLegacyStatsMemberTypeString;
    case StatsReport::Value::kBool:
      return blink::kWebRTCLegacyStatsMemberTypeBool;
    case StatsReport::Value::kId:
      return blink::kWebRTCLegacyStatsMemberTypeId;
  }
  NOTREACHED();
  return blink::kWebRTCLegacyStatsMemberTypeInt;
}
