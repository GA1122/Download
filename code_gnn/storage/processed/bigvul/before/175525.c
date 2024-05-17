IHEVCD_ERROR_T ihevcd_parse_time_code_sei(codec_t *ps_codec)
{
 parse_ctxt_t *ps_parse = &ps_codec->s_parse;
 bitstrm_t *ps_bitstrm = &ps_parse->s_bitstrm;
    UWORD32 value;
 time_code_t *ps_time_code;
    WORD32 i;

    ps_parse->s_sei_params.i1_time_code_present_flag = 1;
    ps_time_code = &ps_parse->s_sei_params.s_time_code;

    BITS_PARSE("num_clock_ts", value, ps_bitstrm, 2);
    ps_time_code->u1_num_clock_ts = value;

 for(i = 0; i < ps_time_code->u1_num_clock_ts; i++)
 {
        BITS_PARSE("clock_timestamp_flag[i]", value, ps_bitstrm, 1);
        ps_time_code->au1_clock_timestamp_flag[i] = value;

 if(ps_time_code->au1_clock_timestamp_flag[i])
 {
            BITS_PARSE("units_field_based_flag[i]", value, ps_bitstrm, 1);
            ps_time_code->au1_units_field_based_flag[i] = value;

            BITS_PARSE("counting_type[i]", value, ps_bitstrm, 5);
            ps_time_code->au1_counting_type[i] = value;

            BITS_PARSE("full_timestamp_flag[i]", value, ps_bitstrm, 1);
            ps_time_code->au1_full_timestamp_flag[i] = value;

            BITS_PARSE("discontinuity_flag[i]", value, ps_bitstrm, 1);
            ps_time_code->au1_discontinuity_flag[i] = value;

            BITS_PARSE("cnt_dropped_flag[i]", value, ps_bitstrm, 1);
            ps_time_code->au1_cnt_dropped_flag[i] = value;

            BITS_PARSE("n_frames[i]", value, ps_bitstrm, 9);
            ps_time_code->au2_n_frames[i] = value;

 if(ps_time_code->au1_full_timestamp_flag[i])
 {
                BITS_PARSE("seconds_value[i]", value, ps_bitstrm, 6);
                ps_time_code->au1_seconds_value[i] = value;

                BITS_PARSE("minutes_value[i]", value, ps_bitstrm, 6);
                ps_time_code->au1_minutes_value[i] = value;

                BITS_PARSE("hours_value[i]", value, ps_bitstrm, 5);
                ps_time_code->au1_hours_value[i] = value;
 }
 else
 {
                BITS_PARSE("seconds_flag[i]", value, ps_bitstrm, 1);
                ps_time_code->au1_seconds_flag[i] = value;

 if(ps_time_code->au1_seconds_flag[i])
 {
                    BITS_PARSE("seconds_value[i]", value, ps_bitstrm, 6);
                    ps_time_code->au1_seconds_value[i] = value;

                    BITS_PARSE("minutes_flag[i]", value, ps_bitstrm, 1);
                    ps_time_code->au1_minutes_flag[i] = value;

 if(ps_time_code->au1_minutes_flag[i])
 {
                        BITS_PARSE("minutes_value[i]", value, ps_bitstrm, 6);
                        ps_time_code->au1_minutes_value[i] = value;

                        BITS_PARSE("hours_flag[i]", value, ps_bitstrm, 1);
                        ps_time_code->au1_hours_flag[i] = value;

 if(ps_time_code->au1_hours_flag[i])
 {
                            BITS_PARSE("hours_value[i]", value, ps_bitstrm, 5);
                            ps_time_code->au1_hours_value[i] = value;
 }
 }
 }
 }

            BITS_PARSE("time_offset_length[i]", value, ps_bitstrm, 5);
            ps_time_code->au1_time_offset_length[i] = value;

 if(ps_time_code->au1_time_offset_length[i] > 0)
 {
                BITS_PARSE("time_offset_value[i]", value, ps_bitstrm,
                           ps_time_code->au1_time_offset_length[i]);
                ps_time_code->au1_time_offset_value[i] = value;
 }
 else
 {
                ps_time_code->au1_time_offset_value[i] = 0;
 }
 }
 }

 return (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
}
