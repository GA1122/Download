static int read_frame_by_row(FILE *f, vpx_image_t *img) {
 size_t nbytes, to_read;
 int    res = 1;
 int plane;

 for (plane = 0; plane < 3; plane++)
 {
 unsigned char *ptr;
 int w = (plane ? (1 + img->d_w) / 2 : img->d_w);
 int h = (plane ? (1 + img->d_h) / 2 : img->d_h);
 int r;

  
 switch (plane)
 {
 case 1:
            ptr = img->planes[img->fmt==VPX_IMG_FMT_YV12? VPX_PLANE_V : VPX_PLANE_U];
 break;
 case 2:
            ptr = img->planes[img->fmt==VPX_IMG_FMT_YV12?VPX_PLANE_U : VPX_PLANE_V];
 break;
 default:
            ptr = img->planes[plane];
 }

 for (r = 0; r < h; r++)
 {
            to_read = w;

            nbytes = fread(ptr, 1, to_read, f);
 if(nbytes != to_read) {
                res = 0;
 if(nbytes > 0)
                    printf("Warning: Read partial frame. Check your width & height!\n");
 break;
 }

            ptr += img->stride[plane];
 }
 if (!res)
 break;
 }

 return res;
}
