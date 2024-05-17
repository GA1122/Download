static int mode_sense_page(SCSIDiskState *s, int page, uint8_t **p_outbuf,
                           int page_control)
{
    BlockDriverState *bdrv = s->bs;
    int cylinders, heads, secs;
    uint8_t *p = *p_outbuf;

     
    switch (page) {
    case 4:  
        if (s->qdev.type == TYPE_ROM) {
            return -1;
        }
        p[0] = 4;
        p[1] = 0x16;
        if (page_control == 1) {  
            break;
        }
         
        bdrv_get_geometry_hint(bdrv, &cylinders, &heads, &secs);
        p[2] = (cylinders >> 16) & 0xff;
        p[3] = (cylinders >> 8) & 0xff;
        p[4] = cylinders & 0xff;
        p[5] = heads & 0xff;
         
        p[6] = (cylinders >> 16) & 0xff;
        p[7] = (cylinders >> 8) & 0xff;
        p[8] = cylinders & 0xff;
         
        p[9] = (cylinders >> 16) & 0xff;
        p[10] = (cylinders >> 8) & 0xff;
        p[11] = cylinders & 0xff;
         
        p[12] = 0;
        p[13] = 200;
         
        p[14] = 0xff;
        p[15] =  0xff;
        p[16] = 0xff;
         
        p[20] = (5400 >> 8) & 0xff;
        p[21] = 5400 & 0xff;
        break;

    case 5:  
        if (s->qdev.type == TYPE_ROM) {
            return -1;
        }
        p[0] = 5;
        p[1] = 0x1e;
        if (page_control == 1) {  
            break;
        }
         
        p[2] = 5000 >> 8;
        p[3] = 5000 & 0xff;
         
        bdrv_get_geometry_hint(bdrv, &cylinders, &heads, &secs);
        p[4] = heads & 0xff;
        p[5] = secs & 0xff;
        p[6] = s->cluster_size * 2;
        p[8] = (cylinders >> 8) & 0xff;
        p[9] = cylinders & 0xff;
         
        p[10] = (cylinders >> 8) & 0xff;
        p[11] = cylinders & 0xff;
         
        p[12] = (cylinders >> 8) & 0xff;
        p[13] = cylinders & 0xff;
         
        p[14] = 0;
        p[15] = 1;
         
        p[16] = 1;
         
        p[17] = 0;
        p[18] = 1;
         
        p[19] = 1;
         
        p[20] = 1;
         
        p[28] = (5400 >> 8) & 0xff;
        p[29] = 5400 & 0xff;
        break;

    case 8:  
        p[0] = 8;
        p[1] = 0x12;
        if (page_control == 1) {  
            break;
        }
        if (bdrv_enable_write_cache(s->bs)) {
            p[2] = 4;  
        }
        break;

    case 0x2a:  
        if (s->qdev.type != TYPE_ROM) {
            return -1;
        }
        p[0] = 0x2a;
        p[1] = 0x14;
        if (page_control == 1) {  
            break;
        }
        p[2] = 3;  
        p[3] = 0;  
        p[4] = 0x7f;  
        p[5] = 0xff;  
        p[6] = 0x2d | (s->tray_locked ? 2 : 0);
         
        p[7] = 0;  
        p[8] = (50 * 176) >> 8;  
        p[9] = (50 * 176) & 0xff;
        p[10] = 0 >> 8;  
        p[11] = 0 & 0xff;
        p[12] = 2048 >> 8;  
        p[13] = 2048 & 0xff;
        p[14] = (16 * 176) >> 8;  
        p[15] = (16 * 176) & 0xff;
        p[18] = (16 * 176) >> 8;  
        p[19] = (16 * 176) & 0xff;
        p[20] = (16 * 176) >> 8;  
        p[21] = (16 * 176) & 0xff;
        break;

    default:
        return -1;
    }

    *p_outbuf += p[1] + 2;
    return p[1] + 2;
}
