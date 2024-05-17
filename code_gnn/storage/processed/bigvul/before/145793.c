static BROTLI_NOINLINE BrotliResult ProcessCommands(BrotliState* s) {
  return ProcessCommandsInternal(0, s);
}
