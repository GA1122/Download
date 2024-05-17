bool omx_video::omx_c2d_conv::convert(int src_fd, void *src_base, void *src_viraddr,
 int dest_fd, void *dest_base, void *dest_viraddr)
{
 int result;
 if (!src_viraddr || !dest_viraddr || !c2dcc || !src_base || !dest_base) {
        DEBUG_PRINT_ERROR("Invalid arguments omx_c2d_conv::convert");
 return false;
 }
    pthread_mutex_lock(&c_lock);
    result =  c2dcc->convertC2D(src_fd, src_base, src_viraddr,
            dest_fd, dest_base, dest_viraddr);
    pthread_mutex_unlock(&c_lock);
    DEBUG_PRINT_LOW("Color convert status %d",result);
 return ((result < 0)?false:true);
}
