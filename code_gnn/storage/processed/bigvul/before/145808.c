static int BROTLI_NOINLINE SafeDecodeDistanceBlockSwitch(BrotliState* s) {
  return DecodeDistanceBlockSwitchInternal(1, s);
}
