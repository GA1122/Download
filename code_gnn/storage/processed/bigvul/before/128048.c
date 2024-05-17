void AwContents::SetDipScale(JNIEnv* env, jobject obj, jfloat dip_scale) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  SetDipScaleInternal(dip_scale);
}
