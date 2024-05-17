 static lba512_t lba512_muldiv(lba512_t block_count, lba512_t mul_by, int div_by)
// static lba512_t lba512_muldiv(lba512_t block_count, lba512_t mul_by,
// 			      int right_shift)
  {
  	lba512_t bc_quot, bc_rem;
  
  	 
	bc_quot = block_count >> div_by;
	bc_rem  = block_count - (bc_quot << div_by);
	return bc_quot * mul_by + ((bc_rem * mul_by) >> div_by);
// 	bc_quot = block_count >> right_shift;
// 	bc_rem  = block_count - (bc_quot << right_shift);
// 	return bc_quot * mul_by + ((bc_rem * mul_by) >> right_shift);
  }