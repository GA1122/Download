void h264bsdSampleAspectRatio(storage_t *pStorage, u32 *sarWidth, u32 *sarHeight)
{

 
    u32 w = 1;
    u32 h = 1;
 

    ASSERT(pStorage);


 if (pStorage->activeSps &&
        pStorage->activeSps->vuiParametersPresentFlag &&
        pStorage->activeSps->vuiParameters &&
        pStorage->activeSps->vuiParameters->aspectRatioPresentFlag )
 {
 switch (pStorage->activeSps->vuiParameters->aspectRatioIdc)
 {
 case ASPECT_RATIO_UNSPECIFIED:  w = 0; h = 0; break;
 case ASPECT_RATIO_1_1:          w = 1; h = 1; break;
 case ASPECT_RATIO_12_11:        w = 12; h = 11; break;
 case ASPECT_RATIO_10_11:        w = 10; h = 11; break;
 case ASPECT_RATIO_16_11:        w = 16; h = 11; break;
 case ASPECT_RATIO_40_33:        w = 40; h = 33; break;
 case ASPECT_RATIO_24_11:        w = 24; h = 11; break;
 case ASPECT_RATIO_20_11:        w = 20; h = 11; break;
 case ASPECT_RATIO_32_11:        w = 32; h = 11; break;
 case ASPECT_RATIO_80_33:        w = 80; h = 33; break;
 case ASPECT_RATIO_18_11:        w = 18; h = 11; break;
 case ASPECT_RATIO_15_11:        w = 15; h = 11; break;
 case ASPECT_RATIO_64_33:        w = 64; h = 33; break;
 case ASPECT_RATIO_160_99:       w = 160; h = 99; break;
 case ASPECT_RATIO_EXTENDED_SAR:
                w = pStorage->activeSps->vuiParameters->sarWidth;
                h = pStorage->activeSps->vuiParameters->sarHeight;
 if ((w == 0) || (h == 0))
                    w = h = 0;
 break;
 default:
                w = 0;
                h = 0;
 break;
 }
 }

  
 *sarWidth = w;
 *sarHeight = h;

}
