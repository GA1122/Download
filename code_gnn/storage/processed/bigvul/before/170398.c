static jboolean Region_equals(JNIEnv* env, jobject clazz, jlong r1Handle, jlong r2Handle)
{
 const SkRegion *r1 = reinterpret_cast<SkRegion*>(r1Handle);
 const SkRegion *r2 = reinterpret_cast<SkRegion*>(r2Handle);
 return boolTojboolean(*r1 == *r2);
}
