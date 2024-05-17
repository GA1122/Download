int register_android_graphics_Bitmap(JNIEnv* env)
{
 return android::AndroidRuntime::registerNativeMethods(env, kClassPathName,
                                gBitmapMethods, SK_ARRAY_COUNT(gBitmapMethods));
}
