static jboolean Region_op2(JNIEnv* env, jobject, jlong dstHandle, jlong region1Handle, jlong region2Handle, jint op) {
 SkRegion* dst = reinterpret_cast<SkRegion*>(dstHandle);
 const SkRegion* region1 = reinterpret_cast<SkRegion*>(region1Handle);
 const SkRegion* region2 = reinterpret_cast<SkRegion*>(region2Handle);
 bool result = dst->op(*region1, *region2, (SkRegion::Op)op);
 return boolTojboolean(result);
}
