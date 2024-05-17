static void RegionIter_destructor(JNIEnv* env, jobject, jlong pairHandle)
{
 RgnIterPair* pair = reinterpret_cast<RgnIterPair*>(pairHandle);
 SkASSERT(pair);
 delete pair;
}
