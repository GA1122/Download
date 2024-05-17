void omx_video::omx_c2d_conv::close()
{
 if (mLibHandle) {
        pthread_mutex_lock(&c_lock);
 if (mConvertClose && c2dcc)
            mConvertClose(c2dcc);
        pthread_mutex_unlock(&c_lock);
        c2dcc = NULL;
 }
}
