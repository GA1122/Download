static void mpeg4_encode_vol_header(MpegEncContext *s,
                                    int vo_number,
                                    int vol_number)
{
    int vo_ver_id;

    if (!CONFIG_MPEG4_ENCODER)
        return;

    if (s->max_b_frames || s->quarter_sample) {
        vo_ver_id  = 5;
        s->vo_type = ADV_SIMPLE_VO_TYPE;
    } else {
        vo_ver_id  = 1;
        s->vo_type = SIMPLE_VO_TYPE;
    }

    put_bits(&s->pb, 16, 0);
    put_bits(&s->pb, 16, 0x100 + vo_number);         
    put_bits(&s->pb, 16, 0);
    put_bits(&s->pb, 16, 0x120 + vol_number);        

    put_bits(&s->pb, 1, 0);              
    put_bits(&s->pb, 8, s->vo_type);     
    if (s->workaround_bugs & FF_BUG_MS) {
        put_bits(&s->pb, 1, 0);          
    } else {
        put_bits(&s->pb, 1, 1);          
        put_bits(&s->pb, 4, vo_ver_id);  
        put_bits(&s->pb, 3, 1);          
    }

    s->aspect_ratio_info = ff_h263_aspect_to_info(s->avctx->sample_aspect_ratio);

    put_bits(&s->pb, 4, s->aspect_ratio_info);  
    if (s->aspect_ratio_info == FF_ASPECT_EXTENDED) {
        av_reduce(&s->avctx->sample_aspect_ratio.num, &s->avctx->sample_aspect_ratio.den,
                   s->avctx->sample_aspect_ratio.num,  s->avctx->sample_aspect_ratio.den, 255);
        put_bits(&s->pb, 8, s->avctx->sample_aspect_ratio.num);
        put_bits(&s->pb, 8, s->avctx->sample_aspect_ratio.den);
    }

    if (s->workaround_bugs & FF_BUG_MS) {
        put_bits(&s->pb, 1, 0);          
    } else {
        put_bits(&s->pb, 1, 1);          
        put_bits(&s->pb, 2, 1);          
        put_bits(&s->pb, 1, s->low_delay);
        put_bits(&s->pb, 1, 0);          
    }

    put_bits(&s->pb, 2, RECT_SHAPE);     
    put_bits(&s->pb, 1, 1);              

    put_bits(&s->pb, 16, s->avctx->time_base.den);
    if (s->time_increment_bits < 1)
        s->time_increment_bits = 1;
    put_bits(&s->pb, 1, 1);              
    put_bits(&s->pb, 1, 0);              
    put_bits(&s->pb, 1, 1);              
    put_bits(&s->pb, 13, s->width);      
    put_bits(&s->pb, 1, 1);              
    put_bits(&s->pb, 13, s->height);     
    put_bits(&s->pb, 1, 1);              
    put_bits(&s->pb, 1, s->progressive_sequence ? 0 : 1);
    put_bits(&s->pb, 1, 1);              
    if (vo_ver_id == 1)
        put_bits(&s->pb, 1, 0);        
    else
        put_bits(&s->pb, 2, 0);        

    put_bits(&s->pb, 1, 0);              
    put_bits(&s->pb, 1, s->mpeg_quant);  

    if (s->mpeg_quant) {
        ff_write_quant_matrix(&s->pb, s->avctx->intra_matrix);
        ff_write_quant_matrix(&s->pb, s->avctx->inter_matrix);
    }

    if (vo_ver_id != 1)
        put_bits(&s->pb, 1, s->quarter_sample);
    put_bits(&s->pb, 1, 1);              
    put_bits(&s->pb, 1, s->rtp_mode ? 0 : 1);  
    put_bits(&s->pb, 1, s->data_partitioning ? 1 : 0);
    if (s->data_partitioning)
        put_bits(&s->pb, 1, 0);          

    if (vo_ver_id != 1) {
        put_bits(&s->pb, 1, 0);          
        put_bits(&s->pb, 1, 0);          
    }
    put_bits(&s->pb, 1, 0);              

    ff_mpeg4_stuffing(&s->pb);

     
    if (!(s->avctx->flags & AV_CODEC_FLAG_BITEXACT)) {
        put_bits(&s->pb, 16, 0);
        put_bits(&s->pb, 16, 0x1B2);     
        avpriv_put_string(&s->pb, LIBAVCODEC_IDENT, 0);
    }
}
