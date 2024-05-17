void process_EXIF (unsigned char * ExifSection, unsigned int length)
{
 int FirstOffset;

 FocalplaneXRes = 0;
 FocalplaneUnits = 0;
 ExifImageWidth = 0;
 NumOrientations = 0;

 if (ShowTags){
        printf("Exif header %d bytes long\n",length);
 }

 {  
 static uchar ExifHeader[] = "Exif\0\0";
 if (memcmp(ExifSection+2, ExifHeader,6)){
 ErrNonfatal("Incorrect Exif header",0,0);
 return;
 }
 }

 if (memcmp(ExifSection+8,"II",2) == 0){
 if (ShowTags) printf("Exif section in Intel order\n");
 MotorolaOrder = 0;
 }else{
 if (memcmp(ExifSection+8,"MM",2) == 0){
 if (ShowTags) printf("Exif section in Motorola order\n");
 MotorolaOrder = 1;
 }else{
 ErrNonfatal("Invalid Exif alignment marker.",0,0);
 return;
 }
 }

 if (Get16u(ExifSection+10) != 0x2a){
 ErrNonfatal("Invalid Exif start (1)",0,0);
 return;
 }

 FirstOffset = Get32u(ExifSection+12);
 if (FirstOffset < 8 || FirstOffset > 16){
 ErrNonfatal("Suspicious offset of first IFD value",0,0);
 return;
 }

 DirWithThumbnailPtrs = NULL;


 ProcessExifDir(ExifSection+8+FirstOffset, ExifSection+8, length-8, 0);

 ImageInfo.ThumbnailAtEnd = ImageInfo.ThumbnailOffset >= ImageInfo.LargestExifOffset ? TRUE : FALSE;
#ifdef SUPERDEBUG
    printf("Thumbnail %s end", (ImageInfo.ThumbnailAtEnd ? "at" : "NOT at"));
#endif
 if (DumpExifMap){
 unsigned a,b;
        printf("Map: %05d- End of exif\n",length-8);
 for (a = 0; a < length - 8; ++a) {
 unsigned char c = *(ExifSection+8+a);
 unsigned pc = isprint(c) ? c : ' ';
            printf("Map: %4d %02x %c", a, c, pc);
 }
 }


 if (FocalplaneXRes != 0){
 ImageInfo.CCDWidth = (float)(ExifImageWidth * FocalplaneUnits / FocalplaneXRes);

 if (ImageInfo.FocalLength.num != 0 && ImageInfo.FocalLength.denom != 0
 && ImageInfo.FocalLength35mmEquiv == 0){
 ImageInfo.FocalLength35mmEquiv = (int)(
 (double)ImageInfo.FocalLength.num / ImageInfo.FocalLength.denom
 / ImageInfo.CCDWidth * 36 + 0.5);
 }
 }
}
