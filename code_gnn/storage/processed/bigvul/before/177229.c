int   ACodec::getAVCLevelFor(
 int width, int height, int rate, int bitrate,
        OMX_VIDEO_AVCPROFILETYPE profile) {
 switch (profile) {
 case OMX_VIDEO_AVCProfileHigh10:
            bitrate = divUp(bitrate, 3000); break;
 case OMX_VIDEO_AVCProfileHigh:
            bitrate = divUp(bitrate, 1250); break;
 default:
            bitrate = divUp(bitrate, 1000); break;
 }

    width = divUp(width, 16);
    height = divUp(height, 16);
 int mbs = width * height;
    rate *= mbs;
 int maxDimension = max(width, height);

 static const int limits[][5] = {
  
 { 1485, 99, 28, 64, OMX_VIDEO_AVCLevel1  },
 { 1485, 99, 28, 128, OMX_VIDEO_AVCLevel1b },
 { 3000, 396, 56, 192, OMX_VIDEO_AVCLevel11 },
 { 6000, 396, 56, 384, OMX_VIDEO_AVCLevel12 },
 { 11880, 396, 56, 768, OMX_VIDEO_AVCLevel13 },
 { 11880, 396, 56, 2000, OMX_VIDEO_AVCLevel2  },
 { 19800, 792, 79, 4000, OMX_VIDEO_AVCLevel21 },
 { 20250, 1620, 113, 4000, OMX_VIDEO_AVCLevel22 },
 { 40500, 1620, 113, 10000, OMX_VIDEO_AVCLevel3  },
 { 108000, 3600, 169, 14000, OMX_VIDEO_AVCLevel31 },
 { 216000, 5120, 202, 20000, OMX_VIDEO_AVCLevel32 },
 { 245760, 8192, 256, 20000, OMX_VIDEO_AVCLevel4  },
 { 245760, 8192, 256, 50000, OMX_VIDEO_AVCLevel41 },
 { 522240, 8704, 263, 50000, OMX_VIDEO_AVCLevel42 },
 { 589824, 22080, 420, 135000, OMX_VIDEO_AVCLevel5  },
 { 983040, 36864, 543, 240000, OMX_VIDEO_AVCLevel51 },
 { 2073600, 36864, 543, 240000, OMX_VIDEO_AVCLevel52 },
 };

 for (size_t i = 0; i < ARRAY_SIZE(limits); i++) {
 const int (&limit)[5] = limits[i];
 if (rate <= limit[0] && mbs <= limit[1] && maxDimension <= limit[2]
 && bitrate <= limit[3]) {
 return limit[4];
 }
 }
 return 0;
}
