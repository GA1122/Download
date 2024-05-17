static void FillIQMatrix(const JpegQuantizationTable* q_table,
                         VAIQMatrixBufferJPEGBaseline* iq_matrix) {
  static_assert(kJpegMaxQuantizationTableNum ==
                    std::extent<decltype(iq_matrix->load_quantiser_table)>(),
                "max number of quantization table mismatched");
  static_assert(
      sizeof(iq_matrix->quantiser_table[0]) == sizeof(q_table[0].value),
      "number of quantization entries mismatched");
  for (size_t i = 0; i < kJpegMaxQuantizationTableNum; i++) {
    if (!q_table[i].valid)
      continue;
    iq_matrix->load_quantiser_table[i] = 1;
    for (size_t j = 0; j < base::size(q_table[i].value); j++)
      iq_matrix->quantiser_table[i][j] = q_table[i].value[j];
  }
}
