unsigned int ScaleForFrameNumber(unsigned int frame, unsigned int val) {
 if (frame < 10)
 return val;
 if (frame < 20)
 return val / 2;
 if (frame < 30)
 return val * 2 / 3;
 if (frame < 40)
 return val / 4;
 if (frame < 50)
 return val * 7 / 8;
 return val;
}
