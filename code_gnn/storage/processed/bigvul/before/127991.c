void AwContents::FindNext(JNIEnv* env, jobject obj, jboolean forward) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  GetFindHelper()->FindNext(forward);
}
