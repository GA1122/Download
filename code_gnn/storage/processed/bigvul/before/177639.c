 static int ClipByte(int value) {
 if (value > 255)
 return 255;
 else if (value < 0)
 return 0;
 return value;
 }
