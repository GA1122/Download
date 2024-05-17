static size_t getFrameSize(unsigned FT) {
 static const size_t kFrameSizeWB[10] = {
 132, 177, 253, 285, 317, 365, 397, 461, 477, 40
 };

 if (FT >= 10) {
 return 1;
 }

 size_t frameSize = kFrameSizeWB[FT];

    frameSize = (frameSize + 7) / 8 + 1;

 return frameSize;
}
