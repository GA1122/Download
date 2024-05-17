WORD32 ih264d_get_dpb_size(dec_seq_params_t *ps_seq)
{
    WORD32 i4_size;
    UWORD8 u1_level_idc;

    u1_level_idc = ps_seq->u1_level_idc;

 switch(u1_level_idc)
 {
 case 10:
            i4_size = 152064;
 break;
 case 11:
            i4_size = 345600;
 break;
 case 12:
            i4_size = 912384;
 break;
 case 13:
            i4_size = 912384;
 break;
 case 20:
            i4_size = 912384;
 break;
 case 21:
            i4_size = 1824768;
 break;
 case 22:
            i4_size = 3110400;
 break;
 case 30:
            i4_size = 3110400;
 break;
 case 31:
            i4_size = 6912000;
 break;
 case 32:
            i4_size = 7864320;
 break;
 case 40:
            i4_size = 12582912;
 break;
 case 41:
            i4_size = 12582912;
 break;
 case 42:
            i4_size = 12582912;
 break;
 case 50:
            i4_size = 42393600;
 break;
 case 51:
            i4_size = 70778880;
 break;
 case 52:
            i4_size = 70778880;
 break;
 default:
            i4_size = 70778880;
 break;
 }

    i4_size /= (ps_seq->u2_frm_wd_in_mbs * (ps_seq->u2_frm_ht_in_mbs << (1 - ps_seq->u1_frame_mbs_only_flag)));
    i4_size /= 384;
    i4_size = MIN(i4_size, 16);
    i4_size = MAX(i4_size, 1);
 return (i4_size);
}
