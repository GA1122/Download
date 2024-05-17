bool omx_video::omx_c2d_conv::get_buffer_size(int port,unsigned int &buf_size)
{
 int cret = 0;
 bool ret = false;
    C2DBuffReq bufferreq;
 if (c2dcc) {
        bufferreq.size = 0;
        pthread_mutex_lock(&c_lock);
        cret = c2dcc->getBuffReq(port,&bufferreq);
        pthread_mutex_unlock(&c_lock);
        DEBUG_PRINT_LOW("Status of getbuffer is %d", cret);
        ret = (cret)?false:true;
        buf_size = bufferreq.size;
 }
 return ret;
}
