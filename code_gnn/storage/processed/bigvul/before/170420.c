static void scale_rgn(SkRegion* dst, const SkRegion& src, float scale) {
 SkRegion tmp;
 SkRegion::Iterator iter(src);

 for (; !iter.done(); iter.next()) {
 SkIRect r;
       scale_rect(&r, iter.rect(), scale);
       tmp.op(r, SkRegion::kUnion_Op);
 }
   dst->swap(tmp);
}
