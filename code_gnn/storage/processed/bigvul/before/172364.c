bool omx_video::omx_c2d_conv::open(unsigned int height,unsigned int width,
 ColorConvertFormat src, ColorConvertFormat dest,unsigned int src_stride)
{
 bool status = false;
    pthread_mutex_lock(&c_lock);
 if (!c2dcc) {
        c2dcc = mConvertOpen(width, height, width, height,
                src,dest,0,src_stride);
 if (c2dcc) {
            src_format = src;
            status = true;
 } else
            DEBUG_PRINT_ERROR("mConvertOpen failed");
 }
    pthread_mutex_unlock(&c_lock);
 return status;
}
