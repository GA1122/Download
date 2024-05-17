static void JNI_SendTabToSelfAndroidBridge_DeleteAllEntries(
    JNIEnv* env,
    const JavaParamRef<jobject>& j_profile) {
  SendTabToSelfModel* model = GetModel(j_profile);
  if (model->IsReady()) {
    model->DeleteAllEntries();
  }
}
