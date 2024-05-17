static FromColorProc ChooseFromColorProc(const SkBitmap& bitmap) {
 switch (bitmap.colorType()) {
 case kN32_SkColorType:
 return bitmap.alphaType() == kPremul_SkAlphaType ? FromColor_D32 : FromColor_D32_Raw;
 case kARGB_4444_SkColorType:
 return bitmap.alphaType() == kPremul_SkAlphaType ? FromColor_D4444 :
 FromColor_D4444_Raw;
 case kRGB_565_SkColorType:
 return FromColor_D565;
 default:
 break;
 }
 return NULL;
}
