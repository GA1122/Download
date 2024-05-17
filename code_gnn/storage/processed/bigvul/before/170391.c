static inline SkRegion* GetSkRegion(JNIEnv* env, jobject regionObject) {
    jlong regionHandle = env->GetLongField(regionObject, gRegion_nativeInstanceFieldID);
 SkRegion* region = reinterpret_cast<SkRegion*>(regionHandle);
 SkASSERT(region != NULL);
 return region;
}
