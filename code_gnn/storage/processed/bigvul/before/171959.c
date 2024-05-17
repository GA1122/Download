static void btpan_jni_cleanup()
{
    pan_disable();
    jni_initialized = false;
}
