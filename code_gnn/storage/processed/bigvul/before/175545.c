WORD32 ihevcd_get_tu_data_size(WORD32 num_luma_samples)
{


    WORD32 tu_data_size;
    WORD32 num_ctb;
    WORD32 num_luma_tu, num_chroma_tu, num_tu;
    num_ctb = num_luma_samples / (MIN_CTB_SIZE * MIN_CTB_SIZE);

    num_luma_tu = num_luma_samples / (MIN_TU_SIZE * MIN_TU_SIZE);
    num_chroma_tu = num_luma_tu >> 1;

    num_tu = num_luma_tu + num_chroma_tu;
    tu_data_size = 0;

  
  
    tu_data_size += (num_ctb + 1) * sizeof(WORD32);

  
    tu_data_size += num_luma_tu * sizeof(UWORD8);

  
    tu_data_size += num_tu * sizeof(tu_t);

  
    tu_data_size += num_tu * (sizeof(WORD8) + sizeof(WORD8));

  
    tu_data_size += num_tu * sizeof(tu_sblk_coeff_data_t);


 return tu_data_size;
}
