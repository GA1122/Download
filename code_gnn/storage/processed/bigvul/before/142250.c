void PasswordGenerationDialogViewAndroid::Show(base::string16& password) {
  JNIEnv* env = base::android::AttachCurrentThread();

   base::string16 explanation_text =
      l10n_util::GetStringUTF16(IDS_PASSWORD_GENERATION_PROMPT);

  Java_PasswordGenerationDialogBridge_showDialog(
      env, java_object_, base::android::ConvertUTF16ToJavaString(env, password),
      base::android::ConvertUTF16ToJavaString(env, explanation_text));
}
