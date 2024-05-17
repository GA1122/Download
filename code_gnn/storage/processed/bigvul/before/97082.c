void opj_tcd_code_block_dec_deallocate (opj_tcd_precinct_t * p_precinct)
{
        OPJ_UINT32 cblkno , l_nb_code_blocks;

        opj_tcd_cblk_dec_t * l_code_block = p_precinct->cblks.dec;
        if (l_code_block) {
                 
                 
                 


                l_nb_code_blocks = p_precinct->block_size / sizeof(opj_tcd_cblk_dec_t);
                 

                for (cblkno = 0; cblkno < l_nb_code_blocks; ++cblkno) {

                        if (l_code_block->data) {
                                opj_free(l_code_block->data);
                                l_code_block->data = 00;
                        }

                        if (l_code_block->segs) {
                                opj_free(l_code_block->segs );
                                l_code_block->segs = 00;
                        }

                        ++l_code_block;
                }

                opj_free(p_precinct->cblks.dec);
                p_precinct->cblks.dec = 00;
        }
}
