  SoftAACEncoder2::~SoftAACEncoder2() {
      aacEncClose(&mAACEncoder);
  
    delete[] mInputFrame;
    mInputFrame = NULL;
//     onReset();
  }