static void copyLine(Color8888* dst, const unsigned char* src, const ColorMapObject* cmap,
 int transparent, int width) {
 for (; width > 0; width--, src++, dst++) {
 if (*src != transparent && *src < cmap->ColorCount) {
 *dst = gifColorToColor8888(cmap->Colors[*src]);
 }
 }
}
