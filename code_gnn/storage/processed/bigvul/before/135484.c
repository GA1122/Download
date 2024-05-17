bool IsPreLollipopAndroid() {
#if defined(OS_ANDROID)
  return (base::android::BuildInfo::GetInstance()->sdk_int() <
          base::android::SDK_VERSION_LOLLIPOP);
#else
  return false;
#endif
}
