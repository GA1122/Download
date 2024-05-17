GBool DCTStream::readDataUnit(DCTHuffTable *dcHuffTable,
			      DCTHuffTable *acHuffTable,
			      int *prevDC, int data[64]) {
  int run, size, amp;
  int c;
  int i, j;

  if ((size = readHuffSym(dcHuffTable)) == 9999) {
    return gFalse;
  }
  if (size > 0) {
    if ((amp = readAmp(size)) == 9999) {
      return gFalse;
    }
  } else {
    amp = 0;
  }
  data[0] = *prevDC += amp;
  for (i = 1; i < 64; ++i) {
    data[i] = 0;
  }
  i = 1;
  while (i < 64) {
    run = 0;
    while ((c = readHuffSym(acHuffTable)) == 0xf0 && run < 0x30) {
      run += 0x10;
    }
    if (c == 9999) {
      return gFalse;
    }
    if (c == 0x00) {
      break;
    } else {
      run += (c >> 4) & 0x0f;
      size = c & 0x0f;
      amp = readAmp(size);
      if (amp == 9999) {
	return gFalse;
      }
      i += run;
      if (i < 64) {
	j = dctZigZag[i++];
	data[j] = amp;
      }
    }
  }
  return gTrue;
}
