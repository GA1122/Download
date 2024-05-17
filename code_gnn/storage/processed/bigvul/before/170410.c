static void Region_scale(JNIEnv* env, jobject region, jfloat scale, jobject dst) {
 SkRegion* rgn = GetSkRegion(env, region);
 if (dst)
        scale_rgn(GetSkRegion(env, dst), *rgn, scale);
 else
        scale_rgn(rgn, *rgn, scale);
}
