GBool DCTStream::readScanInfo() {
  int length;
  int id, c;
  int i, j;

  length = read16() - 2;
  scanInfo.numComps = str->getChar();
  if (scanInfo.numComps <= 0 || scanInfo.numComps > 4) {
    error(errSyntaxError, getPos(), "Bad number of components in DCT stream");
    scanInfo.numComps = 0;
    return gFalse;
  }
  --length;
  if (length != 2 * scanInfo.numComps + 3) {
    error(errSyntaxError, getPos(), "Bad DCT scan info block");
    return gFalse;
  }
  interleaved = scanInfo.numComps == numComps;
  for (j = 0; j < numComps; ++j) {
    scanInfo.comp[j] = gFalse;
    scanInfo.dcHuffTable[j] = 0;
    scanInfo.acHuffTable[j] = 0;
  }
  for (i = 0; i < scanInfo.numComps; ++i) {
    id = str->getChar();
    if (id == compInfo[i].id) {
      j = i;
    } else {
      for (j = 0; j < numComps; ++j) {
	if (id == compInfo[j].id) {
	  break;
	}
      }
      if (j == numComps) {
	error(errSyntaxError, getPos(),
	      "Bad DCT component ID in scan info block");
	return gFalse;
      }
    }
    scanInfo.comp[j] = gTrue;
    c = str->getChar();
    scanInfo.dcHuffTable[j] = (c >> 4) & 0x0f;
    scanInfo.acHuffTable[j] = c & 0x0f;
  }
  scanInfo.firstCoeff = str->getChar();
  scanInfo.lastCoeff = str->getChar();
  if (scanInfo.firstCoeff < 0 || scanInfo.lastCoeff > 63 ||
      scanInfo.firstCoeff > scanInfo.lastCoeff) {
    error(errSyntaxError, getPos(),
	  "Bad DCT coefficient numbers in scan info block");
    return gFalse;
  }
  c = str->getChar();
  scanInfo.ah = (c >> 4) & 0x0f;
  scanInfo.al = c & 0x0f;
  return gTrue;
}
