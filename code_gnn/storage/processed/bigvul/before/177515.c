 static void get_image_md5(const vpx_image_t *img, unsigned char digest[16]) {
 int plane, y;
  MD5Context md5;

  MD5Init(&md5);

 for (plane = 0; plane < 3; ++plane) {
 const unsigned char *buf = img->planes[plane];
 const int stride = img->stride[plane];
 const int w = plane ? (img->d_w + 1) >> 1 : img->d_w;
 const int h = plane ? (img->d_h + 1) >> 1 : img->d_h;

 for (y = 0; y < h; ++y) {
      MD5Update(&md5, buf, w);
      buf += stride;
 }
 }

  MD5Final(digest, &md5);
}
