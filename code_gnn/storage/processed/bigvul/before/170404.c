static jboolean Region_op0(JNIEnv* env, jobject, jlong dstHandle, jint left, jint top, jint right, jint bottom, jint op) {
 SkRegion* dst = reinterpret_cast<SkRegion*>(dstHandle);
 SkIRect ir;

    ir.set(left, top, right, bottom);
 bool result = dst->op(ir, (SkRegion::Op)op);
 return boolTojboolean(result);
}
