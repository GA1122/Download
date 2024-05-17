static void vnc_zrle_start(VncState *vs)
{
    buffer_reset(&vs->zrle.zrle);

     
    vs->zrle.tmp = vs->output;
    vs->output = vs->zrle.zrle;
}
