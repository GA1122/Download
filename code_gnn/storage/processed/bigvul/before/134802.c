int32_t AbsCodeToMtCode(int32_t code) {
  switch (code) {
    case ABS_X:
      return ABS_MT_POSITION_X;
    case ABS_Y:
      return ABS_MT_POSITION_Y;
    case ABS_PRESSURE:
      return ABS_MT_PRESSURE;
    case ABS_DISTANCE:
      return ABS_MT_DISTANCE;
    default:
      return -1;
  }
}
