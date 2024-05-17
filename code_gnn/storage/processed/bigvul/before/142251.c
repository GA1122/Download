PasswordGenerationDialogViewAndroid::~PasswordGenerationDialogViewAndroid() {
  DCHECK(!java_object_.is_null());
  Java_PasswordGenerationDialogBridge_destroy(
      base::android::AttachCurrentThread(), java_object_);
}
