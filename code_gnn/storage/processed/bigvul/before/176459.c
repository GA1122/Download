static Color8888 gifColorToColor8888(const GifColorType& color) {
 return ARGB_TO_COLOR8888(0xff, color.Red, color.Green, color.Blue);
}
