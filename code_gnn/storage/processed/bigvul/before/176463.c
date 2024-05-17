static void setLineColor(Color8888* dst, Color8888 color, int width) {
 for (; width > 0; width--, dst++) {
 *dst = color;
 }
}
