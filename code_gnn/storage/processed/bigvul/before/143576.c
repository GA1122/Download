void RecordNearOomDetectionEndReason(NearOomDetectionEndReason reason) {
  UMA_HISTOGRAM_ENUMERATION(
      "Memory.Experimental.OomIntervention.NearOomDetectionEndReason", reason,
      NearOomDetectionEndReason::COUNT);
}
