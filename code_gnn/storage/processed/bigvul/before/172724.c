WORD32 ih264d_access_unit_delimiter_rbsp(dec_struct_t * ps_dec)
{
    UWORD8 u1_primary_pic_type;
    u1_primary_pic_type = ih264d_get_bits_h264(ps_dec->ps_bitstrm, 3);
 switch(u1_primary_pic_type)
 {
 case I_PIC:
 case SI_PIC:
 case ISI_PIC:
            ps_dec->ps_dec_err_status->u1_pic_aud_i = PIC_TYPE_I;
 break;
 default:
            ps_dec->ps_dec_err_status->u1_pic_aud_i = PIC_TYPE_UNKNOWN;
 }
 return (0);
}
