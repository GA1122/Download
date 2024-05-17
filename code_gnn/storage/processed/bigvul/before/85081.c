static void decode_block(decoder_info_t *decoder_info,int size,int ypos,int xpos,int sub){

  int width = decoder_info->width;
  int height = decoder_info->height;
  int xposY = xpos;
  int yposY = ypos;
  int xposC = xpos >> sub;
  int yposC = ypos >> sub;
  int sizeY = size;
  int sizeC = size >> sub;

  block_mode_t mode;
  intra_mode_t intra_mode;

  frame_type_t frame_type = decoder_info->frame_info.frame_type;
  int bipred = decoder_info->bipred;

  int qpY = decoder_info->frame_info.qpb;
  int qpC = sub ? chroma_qp[qpY] : qpY;

   
  SAMPLE *pblock_y = thor_alloc(MAX_SB_SIZE*MAX_SB_SIZE*sizeof(SAMPLE), 32);
  SAMPLE *pblock_u = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
  SAMPLE *pblock_v = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
  int16_t *coeff_y = thor_alloc(2*MAX_TR_SIZE*MAX_TR_SIZE, 32);
  int16_t *coeff_u = thor_alloc(2*MAX_TR_SIZE*MAX_TR_SIZE, 32);
  int16_t *coeff_v = thor_alloc(2*MAX_TR_SIZE*MAX_TR_SIZE, 32);

   
  SAMPLE *pblock0_y = thor_alloc(MAX_SB_SIZE*MAX_SB_SIZE*sizeof(SAMPLE), 32);
  SAMPLE *pblock0_u = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
  SAMPLE *pblock0_v = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
  SAMPLE *pblock1_y = thor_alloc(MAX_SB_SIZE*MAX_SB_SIZE*sizeof(SAMPLE), 32);
  SAMPLE *pblock1_u = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
  SAMPLE *pblock1_v = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
  yuv_frame_t *rec = decoder_info->rec;
  yuv_frame_t *ref = decoder_info->ref[0];


   
  SAMPLE *rec_y = &rec->y[yposY*rec->stride_y+xposY];
  SAMPLE *rec_u = &rec->u[yposC*rec->stride_c+xposC];
  SAMPLE *rec_v = &rec->v[yposC*rec->stride_c+xposC];

  stream_t *stream = decoder_info->stream;

   
  block_info_dec_t block_info;
  block_info.block_pos.size = size;
  block_info.block_pos.ypos = ypos;
  block_info.block_pos.xpos = xpos;
  block_info.coeffq_y = coeff_y;
  block_info.coeffq_u = coeff_u;
  block_info.coeffq_v = coeff_v;
  block_info.sub = sub;

   
  int bwidth = min(size,width - xpos);
  int bheight = min(size,height - ypos);
  block_info.block_pos.bwidth = bwidth;
  block_info.block_pos.bheight = bheight;

  read_block(decoder_info,stream,&block_info,frame_type);
  mode = block_info.block_param.mode;

  if (mode == MODE_INTRA){
    int ql = decoder_info->qmtx ? qp_to_qlevel(qpY,decoder_info->qmtx_offset) : 0;
    intra_mode = block_info.block_param.intra_mode;
    int bwidth = size;  
    int bheight = size;  
    int upright_available = get_upright_available(yposY, xposY, bwidth, bheight, width, height, 1 << decoder_info->log2_sb_size);
    int downleft_available = get_downleft_available(yposY, xposY, bwidth, bheight, width, height, 1 << decoder_info->log2_sb_size);

    int tb_split = block_info.block_param.tb_split;
    decode_and_reconstruct_block_intra(rec_y,rec->stride_y,sizeY,qpY,pblock_y,coeff_y,tb_split,upright_available,downleft_available,intra_mode,yposY,xposY,width,0,decoder_info->bitdepth,decoder_info->qmtx ? decoder_info->iwmatrix[ql][0][1] : NULL);
    if (decoder_info->subsample != 400)
      decode_and_reconstruct_block_intra_uv(rec_u,rec_v,rec->stride_c,sizeC,qpC,pblock_u,pblock_v,coeff_u,coeff_v,tb_split && sizeC > 4,upright_available,downleft_available,intra_mode,yposC,xposC,width>>sub,1,decoder_info->bitdepth,decoder_info->qmtx ? decoder_info->iwmatrix[ql][1][1] : NULL, decoder_info->cfl_intra ? pblock_y : 0, rec_y, rec->stride_y, sub);
  }
  else
  {
    int tb_split = block_info.block_param.tb_split;
    if (mode==MODE_SKIP){
      if (block_info.block_param.dir==2){
        SAMPLE *pblock0_y = thor_alloc(MAX_SB_SIZE*MAX_SB_SIZE*sizeof(SAMPLE), 32);
        SAMPLE *pblock0_u = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
        SAMPLE *pblock0_v = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
        SAMPLE *pblock1_y = thor_alloc(MAX_SB_SIZE*MAX_SB_SIZE*sizeof(SAMPLE), 32);
        SAMPLE *pblock1_u = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
        SAMPLE *pblock1_v = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);

        int r0 = decoder_info->frame_info.ref_array[block_info.block_param.ref_idx0];
        yuv_frame_t *ref0 = r0 >= 0 ? decoder_info->ref[r0] : decoder_info->interp_frames[0];
        int sign0 = ref0->frame_num >= rec->frame_num;
        int r1 = decoder_info->frame_info.ref_array[block_info.block_param.ref_idx1];
        yuv_frame_t *ref1 = r1 >= 0 ? decoder_info->ref[r1] : decoder_info->interp_frames[0];
        int sign1 = ref1->frame_num >= rec->frame_num;
        if (decoder_info->bit_count.stat_frame_type == B_FRAME && decoder_info->interp_ref == 2 && block_info.block_param.skip_idx==0) {
          TEMPLATE(get_inter_prediction_temp)(width, height, ref0, ref1, &block_info.block_pos, decoder_info->deblock_data, decoder_info->num_reorder_pics + 1, decoder_info->frame_info.phase, pblock_y, pblock_u, pblock_v);
        }
        else {
          TEMPLATE(get_inter_prediction_yuv)(ref0, pblock0_y, pblock0_u, pblock0_v, &block_info.block_pos, block_info.block_param.mv_arr0, sign0, width, height, bipred, 0, decoder_info->bitdepth);
          TEMPLATE(get_inter_prediction_yuv)(ref1, pblock1_y, pblock1_u, pblock1_v, &block_info.block_pos, block_info.block_param.mv_arr1, sign1, width, height, bipred, 0, decoder_info->bitdepth);
          TEMPLATE(average_blocks_all)(pblock_y, pblock_u, pblock_v, pblock0_y, pblock0_u, pblock0_v, pblock1_y, pblock1_u, pblock1_v, &block_info.block_pos, sub);
        }
        thor_free(pblock0_y);
        thor_free(pblock0_u);
        thor_free(pblock0_v);
        thor_free(pblock1_y);
        thor_free(pblock1_u);
        thor_free(pblock1_v);
      }
      else{
        int ref_idx = block_info.block_param.ref_idx0;  
        int r = decoder_info->frame_info.ref_array[ref_idx];
        ref = r>=0 ? decoder_info->ref[r] : decoder_info->interp_frames[0];
        int sign = ref->frame_num > rec->frame_num;

        TEMPLATE(get_inter_prediction_yuv)(ref, pblock_y, pblock_u, pblock_v, &block_info.block_pos, block_info.block_param.mv_arr0, sign, width, height, bipred, 0, decoder_info->bitdepth);
      }
      int j;
      for (j = 0; j<bheight; j++) {
        memcpy(&rec_y[j*rec->stride_y], &pblock_y[j*sizeY], bwidth*sizeof(SAMPLE));
      }
      for (j = 0; j<bheight >> sub; j++) {
        memcpy(&rec_u[j*rec->stride_c], &pblock_u[j*sizeC], (bwidth >> sub)*sizeof(SAMPLE));
        memcpy(&rec_v[j*rec->stride_c], &pblock_v[j*sizeC], (bwidth >> sub)*sizeof(SAMPLE));
      }
      copy_deblock_data(decoder_info, &block_info);
      return;
    }
    else if (mode==MODE_MERGE){
      if (block_info.block_param.dir==2){

        SAMPLE *pblock0_y = thor_alloc(MAX_SB_SIZE*MAX_SB_SIZE*sizeof(SAMPLE), 32);
        SAMPLE *pblock0_u = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
        SAMPLE *pblock0_v = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
        SAMPLE *pblock1_y = thor_alloc(MAX_SB_SIZE*MAX_SB_SIZE*sizeof(SAMPLE), 32);
        SAMPLE *pblock1_u = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
        SAMPLE *pblock1_v = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);

        int r0 = decoder_info->frame_info.ref_array[block_info.block_param.ref_idx0];
        yuv_frame_t *ref0 = r0 >= 0 ? decoder_info->ref[r0] : decoder_info->interp_frames[0];
        int sign0 = ref0->frame_num >= rec->frame_num;
        TEMPLATE(get_inter_prediction_yuv)(ref0, pblock0_y, pblock0_u, pblock0_v, &block_info.block_pos, block_info.block_param.mv_arr0, sign0, width, height, bipred, 0, decoder_info->bitdepth);

        int r1 = decoder_info->frame_info.ref_array[block_info.block_param.ref_idx1];
        yuv_frame_t *ref1 = r1 >= 0 ? decoder_info->ref[r1] : decoder_info->interp_frames[0];
        int sign1 = ref1->frame_num >= rec->frame_num;
        TEMPLATE(get_inter_prediction_yuv)(ref1, pblock1_y, pblock1_u, pblock1_v, &block_info.block_pos, block_info.block_param.mv_arr1, sign1, width, height, bipred, 0, decoder_info->bitdepth);

        TEMPLATE(average_blocks_all)(pblock_y, pblock_u, pblock_v, pblock0_y, pblock0_u, pblock0_v, pblock1_y, pblock1_u, pblock1_v, &block_info.block_pos, sub);

        thor_free(pblock0_y);
        thor_free(pblock0_u);
        thor_free(pblock0_v);
        thor_free(pblock1_y);
        thor_free(pblock1_u);
        thor_free(pblock1_v);
      }
      else{
        int ref_idx = block_info.block_param.ref_idx0;  
        int r = decoder_info->frame_info.ref_array[ref_idx];
        ref = r>=0 ? decoder_info->ref[r] : decoder_info->interp_frames[0];
        int sign = ref->frame_num > rec->frame_num;
        TEMPLATE(get_inter_prediction_yuv)(ref, pblock_y, pblock_u, pblock_v, &block_info.block_pos, block_info.block_param.mv_arr0, sign, width, height, bipred, 0, decoder_info->bitdepth);
      }
    }
    else if (mode == MODE_INTER){
      int ref_idx = block_info.block_param.ref_idx0;
      int r = decoder_info->frame_info.ref_array[ref_idx];
      ref = r>=0 ? decoder_info->ref[r] : decoder_info->interp_frames[0];
      int sign = ref->frame_num > rec->frame_num;
      TEMPLATE(get_inter_prediction_yuv)(ref, pblock_y, pblock_u, pblock_v, &block_info.block_pos, block_info.block_param.mv_arr0, sign, width, height, bipred, decoder_info->pb_split, decoder_info->bitdepth);
    }
    else if (mode == MODE_BIPRED){

      SAMPLE *pblock0_y = thor_alloc(MAX_SB_SIZE*MAX_SB_SIZE*sizeof(SAMPLE), 32);
      SAMPLE *pblock0_u = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
      SAMPLE *pblock0_v = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
      SAMPLE *pblock1_y = thor_alloc(MAX_SB_SIZE*MAX_SB_SIZE*sizeof(SAMPLE), 32);
      SAMPLE *pblock1_u = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);
      SAMPLE *pblock1_v = thor_alloc((MAX_SB_SIZE*MAX_SB_SIZE >> 2*sub)*sizeof(SAMPLE), 32);

      int r0 = decoder_info->frame_info.ref_array[block_info.block_param.ref_idx0];
      yuv_frame_t *ref0 = r0 >= 0 ? decoder_info->ref[r0] : decoder_info->interp_frames[0];
      int sign0 = ref0->frame_num >= rec->frame_num;
      TEMPLATE(get_inter_prediction_yuv)(ref0, pblock0_y, pblock0_u, pblock0_v, &block_info.block_pos, block_info.block_param.mv_arr0, sign0, width, height, bipred, decoder_info->pb_split, decoder_info->bitdepth);

      int r1 = decoder_info->frame_info.ref_array[block_info.block_param.ref_idx1];
      yuv_frame_t *ref1 = r1 >= 0 ? decoder_info->ref[r1] : decoder_info->interp_frames[0];
      int sign1 = ref1->frame_num >= rec->frame_num;
      TEMPLATE(get_inter_prediction_yuv)(ref1, pblock1_y, pblock1_u, pblock1_v, &block_info.block_pos, block_info.block_param.mv_arr1, sign1, width, height, bipred, decoder_info->pb_split, decoder_info->bitdepth);

      TEMPLATE(average_blocks_all)(pblock_y, pblock_u, pblock_v, pblock0_y, pblock0_u, pblock0_v, pblock1_y, pblock1_u, pblock1_v, &block_info.block_pos, sub);

      thor_free(pblock0_y);
      thor_free(pblock0_u);
      thor_free(pblock0_v);
      thor_free(pblock1_y);
      thor_free(pblock1_u);
      thor_free(pblock1_v);
    }

     
    int ql = decoder_info->qmtx ? qp_to_qlevel(qpY,decoder_info->qmtx_offset) : 0;
    decode_and_reconstruct_block_inter(rec_y,rec->stride_y,sizeY,qpY,pblock_y,coeff_y,tb_split,decoder_info->bitdepth,decoder_info->qmtx ? decoder_info->iwmatrix[ql][0][0] : NULL);
    if (decoder_info->cfl_inter && decoder_info->subsample != 400)
      TEMPLATE(improve_uv_prediction)(pblock_y, pblock_u, pblock_v, rec_y, sizeY, sizeY, rec->stride_y, sub, decoder_info->bitdepth);
    decode_and_reconstruct_block_inter(rec_u,rec->stride_c,sizeC,qpC,pblock_u,coeff_u,tb_split&&sizeC>4,decoder_info->bitdepth,decoder_info->qmtx ? decoder_info->iwmatrix[ql][1][0] : NULL);
    decode_and_reconstruct_block_inter(rec_v,rec->stride_c,sizeC,qpC,pblock_v,coeff_v,tb_split&&sizeC>4,decoder_info->bitdepth,decoder_info->qmtx ? decoder_info->iwmatrix[ql][2][0] : NULL);
  }

   
  copy_deblock_data(decoder_info,&block_info);

  thor_free(pblock0_y);
  thor_free(pblock0_u);
  thor_free(pblock0_v);
  thor_free(pblock1_y);
  thor_free(pblock1_u);
  thor_free(pblock1_v);
  thor_free(pblock_y);
  thor_free(pblock_u);
  thor_free(pblock_v);
  thor_free(coeff_y);
  thor_free(coeff_u);
  thor_free(coeff_v);
}