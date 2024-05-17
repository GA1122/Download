void ShowImageInfo(int ShowFileInfo)
{
 if (ShowFileInfo){
        printf("File name    : %s\n",ImageInfo.FileName);
        printf("File size    : %d bytes\n",ImageInfo.FileSize);

 {
 char Temp[20];
 FileTimeAsString(Temp);
            printf("File date    : %s\n",Temp);
 }
 }

 if (ImageInfo.CameraMake[0]){
        printf("Camera make  : %s\n",ImageInfo.CameraMake);
        printf("Camera model : %s\n",ImageInfo.CameraModel);
 }
 if (ImageInfo.DateTime[0]){
        printf("Date/Time    : %s\n",ImageInfo.DateTime);
 }
    printf("Resolution   : %d x %d\n",ImageInfo.Width, ImageInfo.Height);

 if (ImageInfo.Orientation > 1){
        printf("Orientation  : %s\n", OrientTab[ImageInfo.Orientation]);
 }

 if (ImageInfo.IsColor == 0){
        printf("Color/bw     : Black and white\n");
 }

 if (ImageInfo.FlashUsed >= 0){
 if (ImageInfo.FlashUsed & 1){
            printf("Flash used   : Yes");
 switch (ImageInfo.FlashUsed){
 case 0x5: printf(" (Strobe light not detected)"); break;
 case 0x7: printf(" (Strobe light detected) "); break;
 case 0x9: printf(" (manual)"); break;
 case 0xd: printf(" (manual, return light not detected)"); break;
 case 0xf: printf(" (manual, return light  detected)"); break;
 case 0x19:printf(" (auto)"); break;
 case 0x1d:printf(" (auto, return light not detected)"); break;
 case 0x1f:printf(" (auto, return light detected)"); break;
 case 0x41:printf(" (red eye reduction mode)"); break;
 case 0x45:printf(" (red eye reduction mode return light not detected)"); break;
 case 0x47:printf(" (red eye reduction mode return light  detected)"); break;
 case 0x49:printf(" (manual, red eye reduction mode)"); break;
 case 0x4d:printf(" (manual, red eye reduction mode, return light not detected)"); break;
 case 0x4f:printf(" (red eye reduction mode, return light detected)"); break;
 case 0x59:printf(" (auto, red eye reduction mode)"); break;
 case 0x5d:printf(" (auto, red eye reduction mode, return light not detected)"); break;
 case 0x5f:printf(" (auto, red eye reduction mode, return light detected)"); break;
 }
 }else{
            printf("Flash used   : No");
 switch (ImageInfo.FlashUsed){
 case 0x18:printf(" (auto)"); break;
 }
 }
        printf("\n");
 }


 if (ImageInfo.FocalLength.num != 0 && ImageInfo.FocalLength.denom != 0) {
        printf("Focal length : %4.1fmm",(double)ImageInfo.FocalLength.num / ImageInfo.FocalLength.denom);
 if (ImageInfo.FocalLength35mmEquiv){
            printf("  (35mm equivalent: %dmm)", ImageInfo.FocalLength35mmEquiv);
 }
        printf("\n");
 }

 if (ImageInfo.DigitalZoomRatio > 1){
        printf("Digital Zoom : %1.3fx\n", (double)ImageInfo.DigitalZoomRatio);
 }

 if (ImageInfo.CCDWidth){
        printf("CCD width    : %4.2fmm\n",(double)ImageInfo.CCDWidth);
 }

 if (ImageInfo.ExposureTime){
 if (ImageInfo.ExposureTime < 0.010){
            printf("Exposure time: %6.4f s ",(double)ImageInfo.ExposureTime);
 }else{
            printf("Exposure time: %5.3f s ",(double)ImageInfo.ExposureTime);
 }
 if (ImageInfo.ExposureTime <= 0.5){
            printf(" (1/%d)",(int)(0.5 + 1/ImageInfo.ExposureTime));
 }
        printf("\n");
 }
 if (ImageInfo.ApertureFNumber){
        printf("Aperture     : f/%3.3f\n",(double)ImageInfo.ApertureFNumber);
 }
 if (ImageInfo.Distance){
 if (ImageInfo.Distance < 0){
            printf("Focus dist.  : Infinite\n");
 }else{
            printf("Focus dist.  : %4.2fm\n",(double)ImageInfo.Distance);
 }
 }

 if (ImageInfo.ISOequivalent){
        printf("ISO equiv.   : %2d\n",(int)ImageInfo.ISOequivalent);
 }

 if (ImageInfo.ExposureBias){
        printf("Exposure bias: %4.2f\n",(double)ImageInfo.ExposureBias);
 }

 switch(ImageInfo.Whitebalance) {
 case 1:
            printf("Whitebalance : Manual\n");
 break;
 case 0:
            printf("Whitebalance : Auto\n");
 break;
 }

 switch(ImageInfo.LightSource) {
 case 1:
            printf("Light Source : Daylight\n");
 break;
 case 2:
            printf("Light Source : Fluorescent\n");
 break;
 case 3:
            printf("Light Source : Incandescent\n");
 break;
 case 4:
            printf("Light Source : Flash\n");
 break;
 case 9:
            printf("Light Source : Fine weather\n");
 break;
 case 11:
            printf("Light Source : Shade\n");
 break;
 default:;  
 }

 if (ImageInfo.MeteringMode){  
 switch(ImageInfo.MeteringMode) {
 case 2:
            printf("Metering Mode: center weight\n");
 break;
 case 3:
            printf("Metering Mode: spot\n");
 break;
 case 5:
            printf("Metering Mode: matrix\n");
 break;
 }
 }

 if (ImageInfo.ExposureProgram){  
 switch(ImageInfo.ExposureProgram) {
 case 1:
            printf("Exposure     : Manual\n");
 break;
 case 2:
            printf("Exposure     : program (auto)\n");
 break;
 case 3:
            printf("Exposure     : aperture priority (semi-auto)\n");
 break;
 case 4:
            printf("Exposure     : shutter priority (semi-auto)\n");
 break;
 case 5:
            printf("Exposure     : Creative Program (based towards depth of field)\n");
 break;
 case 6:
            printf("Exposure     : Action program (based towards fast shutter speed)\n");
 break;
 case 7:
            printf("Exposure     : Portrait Mode\n");
 break;
 case 8:
            printf("Exposure     : LandscapeMode \n");
 break;
 default:
 break;
 }
 }
 switch(ImageInfo.ExposureMode){
 case 0:  
 break;
 case 1: printf("Exposure Mode: Manual\n");
 break;
 case 2: printf("Exposure Mode: Auto bracketing\n");
 break;
 }

 if (ImageInfo.DistanceRange) {
        printf("Focus range  : ");
 switch(ImageInfo.DistanceRange) {
 case 1:
                printf("macro");
 break;
 case 2:
                printf("close");
 break;
 case 3:
                printf("distant");
 break;
 }
        printf("\n");
 }



 if (ImageInfo.Process != M_SOF0){
 int a;
 for (a=0;;a++){
 if (a >= (int)PROCESS_TABLE_SIZE){
                printf("Jpeg process : Unknown\n");
 break;
 }
 if (ProcessTable[a].Tag == ImageInfo.Process){
                printf("Jpeg process : %s\n",ProcessTable[a].Desc);
 break;
 }
 }
 }

 if (ImageInfo.GpsInfoPresent){
        printf("GPS Latitude : %s\n",ImageInfo.GpsLat);
        printf("GPS Longitude: %s\n",ImageInfo.GpsLong);
 if (ImageInfo.GpsAlt[0]) printf("GPS Altitude : %s\n",ImageInfo.GpsAlt);
 }

 if (ImageInfo.Comments[0]){
 int a,c;
        printf("Comment      : ");
 if (!ImageInfo.CommentWidchars){
 for (a=0;a<MAX_COMMENT_SIZE;a++){
                c = ImageInfo.Comments[a];
 if (c == '\0') break;
 if (c == '\n'){
 if (ImageInfo.Comments[a+1] != '\0'){
                        printf("\nComment      : ");
 }else{
                        printf("\n");
 }
 }else{
                    putchar(c);
 }
 }
            printf("\n");
 }else{
            printf("%.*ls\n", ImageInfo.CommentWidchars, (wchar_t *)ImageInfo.Comments);
 }
 }
 if (ImageInfo.ThumbnailOffset){
        printf("Map: %05d-%05d: Thumbnail\n",ImageInfo.ThumbnailOffset, ImageInfo.ThumbnailOffset+ImageInfo.ThumbnailSize);
 } else {
        printf("NO thumbnail");
 }
}
