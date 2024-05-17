static void decode_and_reconstruct_block_inter (SAMPLE *rec, int stride, int size, int qp, SAMPLE *pblock,
                                                int16_t *coeffq,int tb_split, int bitdepth, qmtx_t ** iwmatrix){

  int16_t *rcoeff = thor_alloc(2*MAX_TR_SIZE*MAX_TR_SIZE, 32);
  int16_t *rblock = thor_alloc(2*MAX_TR_SIZE*MAX_TR_SIZE, 32);
  int16_t *rblock2 = thor_alloc(2*MAX_TR_SIZE*MAX_TR_SIZE, 32);

  if (tb_split){
    int size2 = size/2;
    int i,j,k,index;
    for (i=0;i<size;i+=size2){
      for (j=0;j<size;j+=size2){
        index = 2*(i/size2) + (j/size2);
        TEMPLATE(dequantize)(coeffq+index*size2*size2, rcoeff, qp, size2, iwmatrix ? iwmatrix[log2i(size2/4)] : NULL);
        inverse_transform (rcoeff, rblock2, size2, bitdepth);
         
        for (k=0;k<size2;k++){
          memcpy(rblock+(i+k)*size+j,rblock2+k*size2,size2*sizeof(int16_t));
        }
      }
    }
  }
  else {
    TEMPLATE(dequantize)(coeffq, rcoeff, qp, size, iwmatrix ? iwmatrix[log2i(size/4)] : NULL);

    inverse_transform (rcoeff, rblock, size, bitdepth);
  }
  TEMPLATE(reconstruct_block)(rblock,pblock,rec,size,size,stride,bitdepth);

  thor_free(rcoeff);
  thor_free(rblock);
  thor_free(rblock2);
}