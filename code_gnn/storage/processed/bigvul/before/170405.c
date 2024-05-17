static jboolean Region_op1(JNIEnv* env, jobject, jlong dstHandle, jobject rectObject, jlong regionHandle, jint op) {
 SkRegion* dst = reinterpret_cast<SkRegion*>(dstHandle);
 const SkRegion* region = reinterpret_cast<SkRegion*>(regionHandle);
 SkIRect    ir;
 GraphicsJNI::jrect_to_irect(env, rectObject, &ir);
 bool result = dst->op(ir, *region, (SkRegion::Op)op);
 return boolTojboolean(result);
}
