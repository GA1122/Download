static void FoldChannels(Format* buf_out,
                         int sample_count,
                         const float volume,
                         int channels) {
  Format* buf_in = buf_out;
  const int center_volume = static_cast<int>(volume * 0.707f * 65536);
  const int fixed_volume = static_cast<int>(volume * 65536);

  for (int i = 0; i < sample_count; ++i) {
    int center = static_cast<int>(buf_in[kChannel_C] - bias);
    int left = static_cast<int>(buf_in[kChannel_L] - bias);
    int right = static_cast<int>(buf_in[kChannel_R] - bias);

    center = ScaleChannel<Fixed>(center, center_volume);
    left = ScaleChannel<Fixed>(left, fixed_volume);
    right = ScaleChannel<Fixed>(right, fixed_volume);

    buf_out[0] = static_cast<Format>(
        AddChannel<Fixed, min_value, max_value>(left, center) + bias);
    buf_out[1] = static_cast<Format>(
        AddChannel<Fixed, min_value, max_value>(right, center) + bias);

    buf_out += 2;
    buf_in += channels;
  }
}
