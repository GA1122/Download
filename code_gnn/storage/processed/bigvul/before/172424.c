bool venc_dev::venc_calibrate_gop()
{
 int ratio, sub_gop_size, gop_size, nPframes, nBframes, nLayers;
 int num_sub_gops_in_a_gop;
    nPframes = intra_period.num_pframes;
    nBframes = intra_period.num_bframes;
    nLayers = hier_layers.numlayers;

 if (!nPframes) {
        DEBUG_PRINT_ERROR("nPframes should be non-zero\n");
 return false;
 }

 if (nLayers > 1) {  
        sub_gop_size = 1 << (nLayers - 1);
  
        gop_size = MAX(sub_gop_size, ROUND(nPframes + nBframes, sub_gop_size));
        num_sub_gops_in_a_gop = gop_size/sub_gop_size;
 if (nBframes) {  
  
            nPframes = num_sub_gops_in_a_gop;
            nBframes = gop_size - nPframes;
 } else {  
  
            nPframes = gop_size - 1;
 }
 } else {  
 if (nBframes) {
  
            ratio = MAX(1, MIN((nBframes + (nPframes >> 1))/nPframes, 3));
            nBframes = ratio * nPframes;
 }
 }
    DEBUG_PRINT_LOW("P/B Frames changed from: %ld/%ld to %d/%d",
        intra_period.num_pframes, intra_period.num_bframes, nPframes, nBframes);
    intra_period.num_pframes = nPframes;
    intra_period.num_bframes = nBframes;
    hier_layers.numlayers = nLayers;
 return true;
}
