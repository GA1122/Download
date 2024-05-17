static GBool isImageInterpolationRequired(int srcWidth, int srcHeight,
                                          int scaledWidth, int scaledHeight,
                                          GBool interpolate) {
  if (interpolate)
    return gTrue;

   
  if (scaledWidth / srcWidth >= 4 || scaledHeight / srcHeight >= 4)
    return gFalse;

  return gTrue;
}
