static ToColorProc ChooseToColorProc(const SkBitmap& src) {
 switch (src.colorType()) {
 case kN32_SkColorType:
 switch (src.alphaType()) {
 case kOpaque_SkAlphaType:
 return ToColor_S32_Opaque;
 case kPremul_SkAlphaType:
 return ToColor_S32_Alpha;
 case kUnpremul_SkAlphaType:
 return ToColor_S32_Raw;
 default:
 return NULL;
 }
 case kARGB_4444_SkColorType:
 switch (src.alphaType()) {
 case kOpaque_SkAlphaType:
 return ToColor_S4444_Opaque;
 case kPremul_SkAlphaType:
 return ToColor_S4444_Alpha;
 case kUnpremul_SkAlphaType:
 return ToColor_S4444_Raw;
 default:
 return NULL;
 }
 case kRGB_565_SkColorType:
 return ToColor_S565;
 case kIndex_8_SkColorType:
 if (src.getColorTable() == NULL) {
 return NULL;
 }
 switch (src.alphaType()) {
 case kOpaque_SkAlphaType:
 return ToColor_SI8_Opaque;
 case kPremul_SkAlphaType:
 return ToColor_SI8_Alpha;
 case kUnpremul_SkAlphaType:
 return ToColor_SI8_Raw;
 default:
 return NULL;
 }
 default:
 break;
 }
 return NULL;
}
