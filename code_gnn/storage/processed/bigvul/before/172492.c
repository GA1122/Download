bool venc_dev::venc_validate_hybridhp_params(OMX_U32 layers, OMX_U32 bFrames, OMX_U32 count, int mode)
{
 if (layers && (mode == HIER_P || mode == HIER_B) && hier_layers.hier_mode == HIER_P_HYBRID)
 return false;

 if (bFrames && hier_layers.hier_mode == HIER_P_HYBRID)
 return false;

 if (layers && mode == HIER_P_HYBRID && (intra_period.num_bframes || hier_layers.hier_mode == HIER_P ||
           hier_layers.hier_mode == HIER_B || ltrinfo.count))
 return false;

 if (count && hier_layers.hier_mode == HIER_P_HYBRID)
 return false;

 return true;
}
