static void JNI_SendTabToSelfAndroidBridge_GetAllGuids(
    JNIEnv* env,
    const JavaParamRef<jobject>& j_profile,
    const JavaParamRef<jobject>& j_guid_list_obj) {
  SendTabToSelfModel* model = GetModel(j_profile);
  if (model->IsReady()) {
    std::vector<std::string> all_ids = model->GetAllGuids();
    for (std::vector<std::string>::iterator it = all_ids.begin();
         it != all_ids.end(); ++it) {
      ScopedJavaLocalRef<jstring> j_guid = ConvertUTF8ToJavaString(env, *it);
      Java_SendTabToSelfAndroidBridge_addToGuidList(env, j_guid_list_obj,
                                                    j_guid);
    }
  }
}
