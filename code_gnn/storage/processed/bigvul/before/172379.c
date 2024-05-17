omx_video::omx_c2d_conv::~omx_c2d_conv()
{
    DEBUG_PRINT_HIGH("Destroy C2D instance");
 if (mLibHandle) {
 if (mConvertClose && c2dcc) {
            pthread_mutex_lock(&c_lock);
            mConvertClose(c2dcc);
            pthread_mutex_unlock(&c_lock);
 }
        dlclose(mLibHandle);
 }
    c2dcc = NULL;
    mLibHandle = NULL;
    mConvertOpen = NULL;
    mConvertClose = NULL;
    pthread_mutex_destroy(&c_lock);
}
