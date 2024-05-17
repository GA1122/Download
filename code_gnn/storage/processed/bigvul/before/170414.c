static void Region_translate(JNIEnv* env, jobject region, jint x, jint y, jobject dst) {
 SkRegion* rgn = GetSkRegion(env, region);
 if (dst)
        rgn->translate(x, y, GetSkRegion(env, dst));
 else
        rgn->translate(x, y);
}
