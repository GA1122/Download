void ConnectionInfoPopupAndroid::SetIdentityInfo(
    const IdentityInfo& identity_info) {
  JNIEnv* env = base::android::AttachCurrentThread();

  {
    int icon_id = ResourceMapper::MapFromChromiumId(
        WebsiteSettingsUI::GetIdentityIconID(identity_info.identity_status));

    std::string headline;
    if (identity_info.cert_id) {
      headline = identity_info.site_identity;
    }

    ScopedJavaLocalRef<jstring> description =
        ConvertUTF8ToJavaString(env, identity_info.identity_status_description);
    base::string16 certificate_label =
        l10n_util::GetStringUTF16(IDS_PAGEINFO_CERT_INFO_BUTTON);
    Java_ConnectionInfoPopup_addCertificateSection(
        env,
        popup_jobject_.obj(),
        icon_id,
        ConvertUTF8ToJavaString(env, headline).obj(),
        description.obj(),
        ConvertUTF16ToJavaString(env, certificate_label).obj());

    if (identity_info.show_ssl_decision_revoke_button) {
      base::string16 reset_button_label = l10n_util::GetStringUTF16(
          IDS_PAGEINFO_RESET_INVALID_CERTIFICATE_DECISIONS_BUTTON);
      Java_ConnectionInfoPopup_addResetCertDecisionsButton(
          env,
          popup_jobject_.obj(),
          ConvertUTF16ToJavaString(env, reset_button_label).obj());
    }
  }

  {
    int icon_id = ResourceMapper::MapFromChromiumId(
        WebsiteSettingsUI::GetConnectionIconID(
            identity_info.connection_status));

    ScopedJavaLocalRef<jstring> description = ConvertUTF8ToJavaString(
        env, identity_info.connection_status_description);
    Java_ConnectionInfoPopup_addDescriptionSection(
        env, popup_jobject_.obj(), icon_id, NULL, description.obj());
  }

  Java_ConnectionInfoPopup_addMoreInfoLink(
      env,
      popup_jobject_.obj(),
      ConvertUTF8ToJavaString(
          env, l10n_util::GetStringUTF8(IDS_PAGE_INFO_HELP_CENTER_LINK)).obj());
  Java_ConnectionInfoPopup_showDialog(env, popup_jobject_.obj());
}
