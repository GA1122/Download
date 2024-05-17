void FormatConverter::Convert(WebGLImageConversion::AlphaOp alpha_op) {
#define FORMATCONVERTER_CASE_ALPHAOP(alphaOp) \
  case alphaOp:                               \
    return Convert<SrcFormat, DstFormat, alphaOp>();

  switch (alpha_op) {
    FORMATCONVERTER_CASE_ALPHAOP(WebGLImageConversion::kAlphaDoNothing)
    FORMATCONVERTER_CASE_ALPHAOP(WebGLImageConversion::kAlphaDoPremultiply)
    FORMATCONVERTER_CASE_ALPHAOP(WebGLImageConversion::kAlphaDoUnmultiply)
    default:
      NOTREACHED();
  }
#undef FORMATCONVERTER_CASE_ALPHAOP
}
