static jlong RegionIter_constructor(JNIEnv* env, jobject, jlong regionHandle)
{
 const SkRegion* region = reinterpret_cast<SkRegion*>(regionHandle);
 SkASSERT(region);
 return reinterpret_cast<jlong>(new RgnIterPair(*region));
}
