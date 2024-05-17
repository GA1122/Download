void ShowConciseImageInfo(void)
{
    printf("\"%s\"",ImageInfo.FileName);

    printf(" %dx%d",ImageInfo.Width, ImageInfo.Height);

 if (ImageInfo.ExposureTime){
 if (ImageInfo.ExposureTime <= 0.5){
            printf(" (1/%d)",(int)(0.5 + 1/ImageInfo.ExposureTime));
 }else{
            printf(" (%1.1f)",ImageInfo.ExposureTime);
 }
 }

 if (ImageInfo.ApertureFNumber){
        printf(" f/%3.1f",(double)ImageInfo.ApertureFNumber);
 }

 if (ImageInfo.FocalLength35mmEquiv){
        printf(" f(35)=%dmm",ImageInfo.FocalLength35mmEquiv);
 }

 if (ImageInfo.FlashUsed >= 0 && ImageInfo.FlashUsed & 1){
        printf(" (flash)");
 }

 if (ImageInfo.IsColor == 0){
        printf(" (bw)");
 }

    printf("\n");
}
