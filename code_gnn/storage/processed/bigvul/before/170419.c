static void scale_rect(SkIRect* dst, const SkIRect& src, float scale) {
   dst->fLeft = (int)::roundf(src.fLeft * scale);
   dst->fTop = (int)::roundf(src.fTop * scale);
   dst->fRight = (int)::roundf(src.fRight * scale);
   dst->fBottom = (int)::roundf(src.fBottom * scale);
}
