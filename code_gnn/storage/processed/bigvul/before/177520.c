static void write_ivf_file_header(FILE *outfile,
 const vpx_codec_enc_cfg_t *cfg,
 int frame_cnt) {
 char header[32];

 if(cfg->g_pass != VPX_RC_ONE_PASS && cfg->g_pass != VPX_RC_LAST_PASS)
 return;
    header[0] = 'D';
    header[1] = 'K';
    header[2] = 'I';
    header[3] = 'F';
    mem_put_le16(header+4, 0);  
    mem_put_le16(header+6, 32);  
    mem_put_le32(header+8,  fourcc);  
    mem_put_le16(header+12, cfg->g_w);  
    mem_put_le16(header+14, cfg->g_h);  
    mem_put_le32(header+16, cfg->g_timebase.den);  
    mem_put_le32(header+20, cfg->g_timebase.num);  
    mem_put_le32(header+24, frame_cnt);  
    mem_put_le32(header+28, 0);  

 (void) fwrite(header, 1, 32, outfile);
}
