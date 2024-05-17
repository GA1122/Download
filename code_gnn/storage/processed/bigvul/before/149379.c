std::string MalwareVerdictToThreatType(
    MalwareDeepScanningVerdict::Verdict verdict) {
  switch (verdict) {
    case MalwareDeepScanningVerdict::CLEAN:
      return "SAFE";
    case MalwareDeepScanningVerdict::UWS:
      return "POTENTIALLY_UNWANTED";
    case MalwareDeepScanningVerdict::MALWARE:
      return "DANGEROUS";
    case MalwareDeepScanningVerdict::VERDICT_UNSPECIFIED:
    default:
      return "UNKNOWN";
  }
}
