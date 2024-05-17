JNI_SendTabToSelfAndroidBridge_GetEntryByGUID(
    JNIEnv* env,
    const JavaParamRef<jobject>& j_profile,
    const JavaParamRef<jstring>& j_guid) {
  SendTabToSelfModel* model = GetModel(j_profile);
  if (!model->IsReady()) {
    return nullptr;
  }

  const std::string guid = ConvertJavaStringToUTF8(env, j_guid);
  const SendTabToSelfEntry* found_entry = model->GetEntryByGUID(guid);

  if (found_entry == nullptr) {
    return nullptr;
  }

  return CreateJavaSendTabToSelfEntry(env, found_entry);
}
