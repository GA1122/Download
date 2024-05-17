WORD32 ihevcd_get_pic_mv_bank_size(WORD32 num_luma_samples)
{
    WORD32 size;

    WORD32 pic_size;

    WORD32 mv_bank_size;
    WORD32 num_pu;
    WORD32 num_ctb;
    pic_size = num_luma_samples;


    num_pu = pic_size / (MIN_PU_SIZE * MIN_PU_SIZE);
    num_ctb = pic_size / (MIN_CTB_SIZE * MIN_CTB_SIZE);

    mv_bank_size = 0;

  
  
    mv_bank_size += (num_ctb + 1) * sizeof(WORD32);

  
    mv_bank_size += num_pu;

  
    mv_bank_size += num_pu * sizeof(pu_t);

  
    mv_bank_size += ALIGN4(num_ctb * sizeof(UWORD16));

    size =  mv_bank_size;
 return size;
}
