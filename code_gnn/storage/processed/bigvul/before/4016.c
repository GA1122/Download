GBool DCTStream::readHeader() {
  GBool doScan;
  int n;
  int c = 0;
  int i;

  doScan = gFalse;
  while (!doScan) {
    c = readMarker();
    switch (c) {
    case 0xc0:			 
    case 0xc1:			 
      if (!readBaselineSOF()) {
	return gFalse;
      }
      break;
    case 0xc2:			 
      if (!readProgressiveSOF()) {
	return gFalse;
      }
      break;
    case 0xc4:			 
      if (!readHuffmanTables()) {
	return gFalse;
      }
      break;
    case 0xd8:			 
      break;
    case 0xd9:			 
      return gFalse;
    case 0xda:			 
      if (!readScanInfo()) {
	return gFalse;
      }
      doScan = gTrue;
      break;
    case 0xdb:			 
      if (!readQuantTables()) {
	return gFalse;
      }
      break;
    case 0xdd:			 
      if (!readRestartInterval()) {
	return gFalse;
      }
      break;
    case 0xe0:			 
      if (!readJFIFMarker()) {
	return gFalse;
      }
      break;
    case 0xee:			 
      if (!readAdobeMarker()) {
	return gFalse;
      }
      break;
    case EOF:
      error(errSyntaxError, getPos(), "Bad DCT header");
      return gFalse;
    default:
      if (c >= 0xe0) {
	n = read16() - 2;
	for (i = 0; i < n; ++i) {
	  str->getChar();
	}
      } else {
	error(errSyntaxError, getPos(), "Unknown DCT marker <{0:02x}>", c);
	return gFalse;
      }
      break;
    }
  }

  return gTrue;
}
