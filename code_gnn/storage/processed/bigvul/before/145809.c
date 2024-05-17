static int BROTLI_NOINLINE SafeDecodeLiteralBlockSwitch(BrotliState* s) {
  return DecodeLiteralBlockSwitchInternal(1, s);
}
