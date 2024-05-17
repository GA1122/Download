static void JNI_SendTabToSelfAndroidBridge_DeleteEntry(
    JNIEnv* env,
    const JavaParamRef<jobject>& j_profile,
    const JavaParamRef<jstring>& j_guid) {
  SendTabToSelfModel* model = GetModel(j_profile);
  if (model->IsReady()) {
    const std::string guid = ConvertJavaStringToUTF8(env, j_guid);
    model->DeleteEntry(guid);
  }
}
