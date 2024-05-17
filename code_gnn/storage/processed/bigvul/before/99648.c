static void ReportToUMA(VAVDADecoderFailure failure) {
  UMA_HISTOGRAM_ENUMERATION("Media.VAVDA.DecoderFailure", failure,
                            VAVDA_DECODER_FAILURES_MAX);
}
