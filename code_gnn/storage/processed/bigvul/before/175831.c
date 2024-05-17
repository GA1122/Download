int vp8_create_decoder_instances(struct frame_buffers *fb, VP8D_CONFIG *oxcf)
{
 if(!fb->use_frame_threads)
 {
  
        fb->pbi[0] = create_decompressor(oxcf);
 if(!fb->pbi[0])
 return VPX_CODEC_ERROR;

#if CONFIG_MULTITHREAD
  
        fb->pbi[0]->max_threads = oxcf->max_threads;
        vp8_decoder_create_threads(fb->pbi[0]);
#endif
 }
 else
 {
  
 }

 return VPX_CODEC_OK;
}
