static void mpeg4_encode_visual_object_header(MpegEncContext *s)
{
    int profile_and_level_indication;
    int vo_ver_id;

    if (s->avctx->profile != FF_PROFILE_UNKNOWN) {
        profile_and_level_indication = s->avctx->profile << 4;
    } else if (s->max_b_frames || s->quarter_sample) {
        profile_and_level_indication = 0xF0;   
    } else {
        profile_and_level_indication = 0x00;   
    }

    if (s->avctx->level != FF_LEVEL_UNKNOWN)
        profile_and_level_indication |= s->avctx->level;
    else
        profile_and_level_indication |= 1;    

    if (profile_and_level_indication >> 4 == 0xF)
        vo_ver_id = 5;
    else
        vo_ver_id = 1;


    put_bits(&s->pb, 16, 0);
    put_bits(&s->pb, 16, VOS_STARTCODE);

    put_bits(&s->pb, 8, profile_and_level_indication);

    put_bits(&s->pb, 16, 0);
    put_bits(&s->pb, 16, VISUAL_OBJ_STARTCODE);

    put_bits(&s->pb, 1, 1);
    put_bits(&s->pb, 4, vo_ver_id);
    put_bits(&s->pb, 3, 1);      

    put_bits(&s->pb, 4, 1);      

    put_bits(&s->pb, 1, 0);      

    ff_mpeg4_stuffing(&s->pb);
}
