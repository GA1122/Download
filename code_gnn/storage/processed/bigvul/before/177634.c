   virtual void SetUp() {
     int i;

    UUT = GetParam();
    memset(input, 0, sizeof(input));
  
 for (i = 0; i < 256; i++) output[i] = ((i & 0xF) < 4 && (i < 64)) ? 0 : -1;
 }
