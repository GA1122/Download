int DCTStream::readHuffSym(DCTHuffTable *table) {
  Gushort code;
  int bit;
  int codeBits;

  code = 0;
  codeBits = 0;
  do {
    if ((bit = readBit()) == EOF) {
      return 9999;
    }
    code = (code << 1) + bit;
    ++codeBits;

    if (code < table->firstCode[codeBits]) {
      break;
    }
    if (code - table->firstCode[codeBits] < table->numCodes[codeBits]) {
      code -= table->firstCode[codeBits];
      return table->sym[table->firstSym[codeBits] + code];
    }
  } while (codeBits < 16);

  error(errSyntaxError, getPos(), "Bad Huffman code in DCT stream");
  return 9999;
}
