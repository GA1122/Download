UWORD32 ih264d_correct_level_idc(UWORD32 u4_level_idc, UWORD32 u4_total_mbs)
{
    UWORD32 u4_max_mbs_allowed;

 switch(u4_level_idc)
 {
 case H264_LEVEL_1_0:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_10;
 break;
 case H264_LEVEL_1_1:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_11;
 break;
 case H264_LEVEL_1_2:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_12;
 break;
 case H264_LEVEL_1_3:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_13;
 break;
 case H264_LEVEL_2_0:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_20;
 break;
 case H264_LEVEL_2_1:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_21;
 break;
 case H264_LEVEL_2_2:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_22;
 break;
 case H264_LEVEL_3_0:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_30;
 break;
 case H264_LEVEL_3_1:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_31;
 break;
 case H264_LEVEL_3_2:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_32;
 break;
 case H264_LEVEL_4_0:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_40;
 break;
 case H264_LEVEL_4_1:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_41;
 break;
 case H264_LEVEL_4_2:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_42;
 break;
 case H264_LEVEL_5_0:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_50;
 break;
 case H264_LEVEL_5_1:
 default:
            u4_max_mbs_allowed = MAX_MBS_LEVEL_51;
 break;

 }

  
 if(u4_total_mbs > u4_max_mbs_allowed)
 {
 if(u4_total_mbs > MAX_MBS_LEVEL_50)
            u4_level_idc = H264_LEVEL_5_1;
 else if(u4_total_mbs > MAX_MBS_LEVEL_42)
            u4_level_idc = H264_LEVEL_5_0;
 else if(u4_total_mbs > MAX_MBS_LEVEL_41)
            u4_level_idc = H264_LEVEL_4_2;
 else if(u4_total_mbs > MAX_MBS_LEVEL_40)
            u4_level_idc = H264_LEVEL_4_1;
 else if(u4_total_mbs > MAX_MBS_LEVEL_32)
            u4_level_idc = H264_LEVEL_4_0;
 else if(u4_total_mbs > MAX_MBS_LEVEL_31)
            u4_level_idc = H264_LEVEL_3_2;
 else if(u4_total_mbs > MAX_MBS_LEVEL_30)
            u4_level_idc = H264_LEVEL_3_1;
 else if(u4_total_mbs > MAX_MBS_LEVEL_21)
            u4_level_idc = H264_LEVEL_3_0;
 else if(u4_total_mbs > MAX_MBS_LEVEL_20)
            u4_level_idc = H264_LEVEL_2_1;
 else if(u4_total_mbs > MAX_MBS_LEVEL_10)
            u4_level_idc = H264_LEVEL_2_0;
 }

 return (u4_level_idc);

}
