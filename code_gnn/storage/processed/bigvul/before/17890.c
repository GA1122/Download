static void vnc_zrle_stop(VncState *vs)
{
     
    vs->zrle.zrle = vs->output;
    vs->output = vs->zrle.tmp;
}
