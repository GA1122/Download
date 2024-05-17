uint8_t clip_pixel(int x) {
 return x < 0 ? 0 :
         x > 255 ? 255 :
         x;
}
