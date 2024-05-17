  static void copyMono24(
          short *dst,
        const int *const *src,
//         const int * src[FLACParser::kMaxChannels],
          unsigned nSamples,
          unsigned  ) {
      for (unsigned i = 0; i < nSamples; ++i) {
  *dst++ = src[0][i] >> 8;
  }
 }