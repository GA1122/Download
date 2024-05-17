static int BROTLI_NOINLINE SafeDecodeCommandBlockSwitch(BrotliState* s) {
  return DecodeCommandBlockSwitchInternal(1, s);
}
