static jboolean IsDownloadDangerous(JNIEnv* env,
                                    const JavaParamRef<jclass>& clazz,
                                    const JavaParamRef<jstring>& filename) {
  base::FilePath path(base::android::ConvertJavaStringToUTF8(env, filename));
  return safe_browsing::FileTypePolicies::GetInstance()->GetFileDangerLevel(
             path) != safe_browsing::DownloadFileType::NOT_DANGEROUS;
}
