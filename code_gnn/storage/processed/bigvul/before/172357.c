bool omx_video::omx_c2d_conv::init()
{
 bool status = true;
 if (mLibHandle || mConvertOpen || mConvertClose) {
        DEBUG_PRINT_ERROR("omx_c2d_conv::init called twice");
        status = false;
 }
 if (status) {
        mLibHandle = dlopen("libc2dcolorconvert.so", RTLD_LAZY);
 if (mLibHandle) {
            mConvertOpen = (createC2DColorConverter_t *)
                dlsym(mLibHandle,"createC2DColorConverter");
            mConvertClose = (destroyC2DColorConverter_t *)
                dlsym(mLibHandle,"destroyC2DColorConverter");
 if (!mConvertOpen || !mConvertClose)
                status = false;
 } else
            status = false;
 }
 if (!status && mLibHandle) {
        dlclose(mLibHandle);
        mLibHandle = NULL;
        mConvertOpen = NULL;
        mConvertClose = NULL;
 }
 return status;
}
