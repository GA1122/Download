void jswrap_graphics_drawImage(JsVar *parent, JsVar *image, int xPos, int yPos) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return;
  if (!jsvIsObject(image)) {
    jsExceptionHere(JSET_ERROR, "Expecting first argument to be an object");
    return;
  }
  int imageWidth = (int)jsvGetIntegerAndUnLock(jsvObjectGetChild(image, "width", 0));
  int imageHeight = (int)jsvGetIntegerAndUnLock(jsvObjectGetChild(image, "height", 0));
  int imageBpp = (int)jsvGetIntegerAndUnLock(jsvObjectGetChild(image, "bpp", 0));
  unsigned int imageBitMask = (unsigned int)((1L<<imageBpp)-1L);
  JsVar *transpVar = jsvObjectGetChild(image, "transparent", 0);
  bool imageIsTransparent = transpVar!=0;
  unsigned int imageTransparentCol = (unsigned int)jsvGetInteger(transpVar);
  jsvUnLock(transpVar);
  JsVar *imageBuffer = jsvObjectGetChild(image, "buffer", 0);
  if (!(jsvIsArrayBuffer(imageBuffer) && imageWidth>0 && imageHeight>0 && imageBpp>0 && imageBpp<=32)) {
    jsExceptionHere(JSET_ERROR, "Expecting first argument to a valid Image");
    jsvUnLock(imageBuffer);
    return;
  }
  JsVar *imageBufferString = jsvGetArrayBufferBackingString(imageBuffer);
  jsvUnLock(imageBuffer);


  int x=0, y=0;
  int bits=0;
  unsigned int colData = 0;
  JsvStringIterator it;
  jsvStringIteratorNew(&it, imageBufferString, 0);
  while ((bits>=imageBpp || jsvStringIteratorHasChar(&it)) && y<imageHeight) {
    while (bits < imageBpp) {
      colData = (colData<<8) | ((unsigned char)jsvStringIteratorGetChar(&it));
      jsvStringIteratorNext(&it);
      bits += 8;
    }
    unsigned int col = (colData>>(bits-imageBpp))&imageBitMask;
    bits -= imageBpp;
    if (!imageIsTransparent || imageTransparentCol!=col) {
      if (imageBpp==1)
        col = col ? gfx.data.fgColor : gfx.data.bgColor;
      graphicsSetPixel(&gfx, (short)(x+xPos), (short)(y+yPos), col);
    }
    x++;
    if (x>=imageWidth) {
      x=0;
      y++;
    }

  }
  jsvStringIteratorFree(&it);
  jsvUnLock(imageBufferString);
  graphicsSetVar(&gfx);  
}
