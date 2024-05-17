omx_video::omx_c2d_conv::omx_c2d_conv()
{
    c2dcc = NULL;
    mLibHandle = NULL;
    mConvertOpen = NULL;
    mConvertClose = NULL;
    src_format = NV12_128m;
    pthread_mutex_init(&c_lock, NULL);
}
