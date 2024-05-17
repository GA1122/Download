int read_block(decoder_info_t *decoder_info,stream_t *stream,block_info_dec_t *block_info, frame_type_t frame_type)
{
  int width = decoder_info->width;
  int height = decoder_info->height;
  int bit_start;
  int code,tb_split;
  int pb_part=0;
  cbp_t cbp;
  int stat_frame_type = decoder_info->bit_count.stat_frame_type;  

  int size = block_info->block_pos.size;
  int ypos = block_info->block_pos.ypos;
  int xpos = block_info->block_pos.xpos;

  YPOS = ypos;
  XPOS = xpos;

  int sizeY = size;
  int sizeC = size>>block_info->sub;

  mv_t mv,zerovec;
  mv_t mvp;
  mv_t mv_arr[4];  
  mv_t mv_arr0[4];
  mv_t mv_arr1[4];

  block_mode_t mode;
  intra_mode_t intra_mode = MODE_DC;

  int16_t *coeff_y = block_info->coeffq_y;
  int16_t *coeff_u = block_info->coeffq_u;
  int16_t *coeff_v = block_info->coeffq_v;

  zerovec.y = zerovec.x = 0;
  bit_start = stream->bitcnt;

  mode = decoder_info->mode;
  int coeff_block_type = (mode == MODE_INTRA)<<1;

   
  bit_start = stream->bitcnt;

  if (mode == MODE_SKIP){
     
    mv_t mv_skip[MAX_NUM_SKIP];
    int num_skip_vec,skip_idx;
    inter_pred_t skip_candidates[MAX_NUM_SKIP];
    num_skip_vec = TEMPLATE(get_mv_skip)(ypos, xpos, width, height, size, size, 1 << decoder_info->log2_sb_size, decoder_info->deblock_data, skip_candidates);
    if (decoder_info->bit_count.stat_frame_type == B_FRAME && decoder_info->interp_ref == 2) {
      num_skip_vec = TEMPLATE(get_mv_skip_temp)(decoder_info->width, decoder_info->frame_info.phase, decoder_info->num_reorder_pics + 1, &block_info->block_pos, decoder_info->deblock_data, skip_candidates);
    }
    for (int idx = 0; idx < num_skip_vec; idx++) {
      mv_skip[idx] = skip_candidates[idx].mv0;
    }
     
    if (num_skip_vec == 4)
      skip_idx = get_flc(2, stream);
    else if (num_skip_vec == 3){
      skip_idx = get_vlc(12, stream);
    }
    else if (num_skip_vec == 2){
      skip_idx = get_flc(1, stream);
    }
    else
      skip_idx = 0;
    decoder_info->bit_count.skip_idx[stat_frame_type] += (stream->bitcnt - bit_start);

    block_info->num_skip_vec = num_skip_vec;
    block_info->block_param.skip_idx = skip_idx;

    if (skip_idx == num_skip_vec)
      mv = mv_skip[0];
    else
      mv = mv_skip[skip_idx];
    mv_arr[0] = mv;
    mv_arr[1] = mv;
    mv_arr[2] = mv;
    mv_arr[3] = mv;

    block_info->block_param.ref_idx0 = skip_candidates[skip_idx].ref_idx0;
    block_info->block_param.ref_idx1 = skip_candidates[skip_idx].ref_idx1;
    for (int i = 0; i < 4; i++) {
      mv_arr0[i] = skip_candidates[skip_idx].mv0;
      mv_arr1[i] = skip_candidates[skip_idx].mv1;
    }
    block_info->block_param.dir = skip_candidates[skip_idx].bipred_flag;
  }
  else if (mode == MODE_MERGE){
     
    mv_t mv_skip[MAX_NUM_SKIP];
    int num_skip_vec,skip_idx;
    inter_pred_t merge_candidates[MAX_NUM_SKIP];
    num_skip_vec = TEMPLATE(get_mv_merge)(ypos, xpos, width, height, size, size, 1 << decoder_info->log2_sb_size, decoder_info->deblock_data, merge_candidates);
    for (int idx = 0; idx < num_skip_vec; idx++) {
      mv_skip[idx] = merge_candidates[idx].mv0;
    }
     
    if (num_skip_vec == 4)
      skip_idx = get_flc(2, stream);
    else if (num_skip_vec == 3){
      skip_idx = get_vlc(12, stream);
    }
    else if (num_skip_vec == 2){
      skip_idx = get_flc(1, stream);
    }
    else
      skip_idx = 0;
    decoder_info->bit_count.skip_idx[stat_frame_type] += (stream->bitcnt - bit_start);

    block_info->num_skip_vec = num_skip_vec;
    block_info->block_param.skip_idx = skip_idx;

    if (skip_idx == num_skip_vec)
      mv = mv_skip[0];
    else
      mv = mv_skip[skip_idx];
    mv_arr[0] = mv;
    mv_arr[1] = mv;
    mv_arr[2] = mv;
    mv_arr[3] = mv;

    block_info->block_param.ref_idx0 = merge_candidates[skip_idx].ref_idx0;
    block_info->block_param.ref_idx1 = merge_candidates[skip_idx].ref_idx1;
    for (int i = 0; i < 4; i++) {
      mv_arr0[i] = merge_candidates[skip_idx].mv0;
      mv_arr1[i] = merge_candidates[skip_idx].mv1;
    }
    block_info->block_param.dir = merge_candidates[skip_idx].bipred_flag;
  }
  else if (mode==MODE_INTER){
    int ref_idx;

    if (decoder_info->pb_split){
       
      pb_part = get_vlc(13, stream);
    }
    else{
      pb_part = 0;
    }
    block_info->block_param.pb_part = pb_part;
    if (decoder_info->frame_info.num_ref > 1){
      ref_idx = decoder_info->ref_idx;
    }
    else{
      ref_idx = 0;
    }

    decoder_info->bit_count.size_and_ref_idx[stat_frame_type][log2i(size)-3][ref_idx] += 1;

    mvp = TEMPLATE(get_mv_pred)(ypos,xpos,width,height,size,size,1<<decoder_info->log2_sb_size,ref_idx,decoder_info->deblock_data);

     
    mv_t mvp2 = mvp;

    if (pb_part==0){
      read_mv(stream,&mv_arr[0],&mvp2);
      mv_arr[1] = mv_arr[0];
      mv_arr[2] = mv_arr[0];
      mv_arr[3] = mv_arr[0];
    }
    else if(pb_part==1){  
      read_mv(stream,&mv_arr[0],&mvp2);
      mvp2 = mv_arr[0];
      read_mv(stream,&mv_arr[2],&mvp2);
      mv_arr[1] = mv_arr[0];
      mv_arr[3] = mv_arr[2];
    }
    else if(pb_part==2){  
      read_mv(stream,&mv_arr[0],&mvp2);
      mvp2 = mv_arr[0];
      read_mv(stream,&mv_arr[1],&mvp2);
      mv_arr[2] = mv_arr[0];
      mv_arr[3] = mv_arr[1];
    }
    else{
      read_mv(stream,&mv_arr[0],&mvp2);
      mvp2 = mv_arr[0];
      read_mv(stream,&mv_arr[1],&mvp2);
      read_mv(stream,&mv_arr[2],&mvp2);
      read_mv(stream,&mv_arr[3],&mvp2);
    }
    decoder_info->bit_count.mv[stat_frame_type] += (stream->bitcnt - bit_start);
    block_info->block_param.ref_idx0 = ref_idx;
    block_info->block_param.ref_idx1 = ref_idx;
    block_info->block_param.dir = 0;
  }
  else if (mode==MODE_BIPRED){
    int ref_idx = 0;
    mvp = TEMPLATE(get_mv_pred)(ypos,xpos,width,height,size,size,1 << decoder_info->log2_sb_size, ref_idx,decoder_info->deblock_data);

     
    mv_t mvp2 = mvp;

#if BIPRED_PART
    if (decoder_info->pb_split) {
       
      pb_part = get_vlc(13, stream);
    }
    else {
      pb_part = 0;
    }
#else
    pb_part = 0;
#endif
    block_info->block_param.pb_part = pb_part;

    if (pb_part == 0) {
      read_mv(stream, &mv_arr0[0], &mvp2);
      mv_arr0[1] = mv_arr0[0];
      mv_arr0[2] = mv_arr0[0];
      mv_arr0[3] = mv_arr0[0];
    }
    else {
      mv_arr0[0] = mvp2;
      mv_arr0[1] = mvp2;
      mv_arr0[2] = mvp2;
      mv_arr0[3] = mvp2;
    }
    if (decoder_info->bit_count.stat_frame_type == B_FRAME)
      mvp2 = mv_arr0[0];
    if (pb_part == 0) {
      read_mv(stream, &mv_arr1[0], &mvp2);
      mv_arr1[1] = mv_arr1[0];
      mv_arr1[2] = mv_arr1[0];
      mv_arr1[3] = mv_arr1[0];
    }
    else if (pb_part == 1) {  
      read_mv(stream, &mv_arr1[0], &mvp2);
      mvp2 = mv_arr1[0];
      read_mv(stream, &mv_arr1[2], &mvp2);
      mv_arr1[1] = mv_arr1[0];
      mv_arr1[3] = mv_arr1[2];
    }
    else if (pb_part == 2) {  
      read_mv(stream, &mv_arr1[0], &mvp2);
      mvp2 = mv_arr1[0];
      read_mv(stream, &mv_arr1[1], &mvp2);
      mv_arr1[2] = mv_arr1[0];
      mv_arr1[3] = mv_arr1[1];
    }
    else {
      read_mv(stream, &mv_arr1[0], &mvp2);
      mvp2 = mv_arr1[0];
      read_mv(stream, &mv_arr1[1], &mvp2);
      read_mv(stream, &mv_arr1[2], &mvp2);
      read_mv(stream, &mv_arr1[3], &mvp2);
    }

    if (decoder_info->bit_count.stat_frame_type == B_FRAME) {
      block_info->block_param.ref_idx0 = 0;
      block_info->block_param.ref_idx1 = 1;
      if (decoder_info->frame_info.interp_ref > 0) {
        block_info->block_param.ref_idx0 += 1;
        block_info->block_param.ref_idx1 += 1;
      }
    }
    else{
      if (decoder_info->frame_info.num_ref == 2) {
        int code = get_vlc(13, stream);
        block_info->block_param.ref_idx0 = (code >> 1) & 1;
        block_info->block_param.ref_idx1 = (code >> 0) & 1;
      }
      else {
        int code = get_vlc(10, stream);
        block_info->block_param.ref_idx0 = (code >> 2) & 3;
        block_info->block_param.ref_idx1 = (code >> 0) & 3;
      }
    }
    block_info->block_param.dir = 2;
    int combined_ref = block_info->block_param.ref_idx0 * decoder_info->frame_info.num_ref + block_info->block_param.ref_idx1;
    decoder_info->bit_count.bi_ref[stat_frame_type][combined_ref] += 1;
    decoder_info->bit_count.mv[stat_frame_type] += (stream->bitcnt - bit_start);
  }

  else if (mode==MODE_INTRA){
     
    if (decoder_info->frame_info.num_intra_modes<=4){
      intra_mode = get_flc(2, stream);
    }
    else {
      intra_mode = get_vlc(8, stream);
    }

    decoder_info->bit_count.intra_mode[stat_frame_type] += (stream->bitcnt - bit_start);
    decoder_info->bit_count.size_and_intra_mode[stat_frame_type][log2i(size)-3][intra_mode] += 1;

    block_info->block_param.intra_mode = intra_mode;
    for (int i=0;i<4;i++){
      mv_arr[i] = zerovec;  
    }
    block_info->block_param.ref_idx0 = 0;
    block_info->block_param.ref_idx1 = 0;
    block_info->block_param.dir = -1;
  }


  if (mode!=MODE_SKIP){
    int tmp;
    int cbp_table[8] = {1,0,5,2,6,3,7,4};
    code = 0;

    if (decoder_info->subsample == 400) {
      tb_split = cbp.u = cbp.v = 0;
      cbp.y = get_flc(1,stream);
      if (decoder_info->tb_split_enable && cbp.y) {
        tb_split = get_flc(1,stream);
        cbp.y &= !tb_split;
      }
    } else {
      bit_start = stream->bitcnt;
      code = get_vlc(0,stream);
      int off = (mode == MODE_MERGE) ? 1 : 2;
      if (decoder_info->tb_split_enable) {
        tb_split = code == off;
        if (code > off) code -= 1;
        if (tb_split)
          decoder_info->bit_count.cbp2_stat[0][stat_frame_type][mode-1][log2i(size)-3][8] += 1;
      }
      else{
        tb_split = 0;
      }
    }
    block_info->block_param.tb_split = tb_split;
    decoder_info->bit_count.cbp[stat_frame_type] += (stream->bitcnt - bit_start);

    if (tb_split == 0){
      if (decoder_info->subsample != 400) {
        tmp = 0;
        if (mode==MODE_MERGE){
          if (code==7)
            code = 1;
          else if (code>0)
            code = code+1;
        }
        else {
          if (decoder_info->block_context->cbp == 0 && code < 2) {
            code = 1 - code;
          }
        }
        while (tmp < 8 && code != cbp_table[tmp]) tmp++;
        decoder_info->bit_count.cbp2_stat[max(0,decoder_info->block_context->cbp)][stat_frame_type][mode-1][log2i(size)-3][tmp] += 1;

        cbp.y = ((tmp>>0)&1);
        cbp.u = ((tmp>>1)&1);
        cbp.v = ((tmp>>2)&1);
      }
      block_info->cbp = cbp;

      if (cbp.y){
        bit_start = stream->bitcnt;
        read_coeff(stream,coeff_y,sizeY,coeff_block_type|0);
        decoder_info->bit_count.coeff_y[stat_frame_type] += (stream->bitcnt - bit_start);
      }
      else
        memset(coeff_y,0,sizeY*sizeY*sizeof(int16_t));

      if (cbp.u){
        bit_start = stream->bitcnt;
        read_coeff(stream,coeff_u,sizeC,coeff_block_type|1);
        decoder_info->bit_count.coeff_u[stat_frame_type] += (stream->bitcnt - bit_start);
      }
      else
        memset(coeff_u,0,sizeC*sizeC*sizeof(int16_t));

      if (cbp.v){
        bit_start = stream->bitcnt;
        read_coeff(stream,coeff_v,sizeC,coeff_block_type|1);
        decoder_info->bit_count.coeff_v[stat_frame_type] += (stream->bitcnt - bit_start);
      }
      else
        memset(coeff_v,0,sizeC*sizeC*sizeof(int16_t));
    }
    else{
      if (sizeC > 4){
        int index;
        int16_t *coeff;

         
        for (index=0;index<4;index++){
          bit_start = stream->bitcnt;
          code = get_vlc(0,stream);
          int tmp = 0;
          while (code != cbp_table[tmp] && tmp < 8) tmp++;
          if (decoder_info->block_context->cbp==0 && tmp < 2)
            tmp = 1-tmp;
          cbp.y = ((tmp>>0)&1);
          cbp.u = ((tmp>>1)&1);
          cbp.v = ((tmp>>2)&1);

           
          decoder_info->bit_count.cbp[stat_frame_type] += (stream->bitcnt - bit_start);
          decoder_info->bit_count.cbp_stat[stat_frame_type][cbp.y + (cbp.u<<1) + (cbp.v<<2)] += 1;

           

           
          coeff = coeff_y + index*sizeY/2*sizeY/2;
          if (cbp.y){
            bit_start = stream->bitcnt;
            read_coeff(stream,coeff,sizeY/2,coeff_block_type|0);
            decoder_info->bit_count.coeff_y[stat_frame_type] += (stream->bitcnt - bit_start);
          }
          else{
            memset(coeff,0,sizeY/2*sizeY/2*sizeof(int16_t));
          }

           
          coeff = coeff_u + index*sizeC/2*sizeC/2;
          if (cbp.u){
            bit_start = stream->bitcnt;
            read_coeff(stream,coeff,sizeC/2,coeff_block_type|1);
            decoder_info->bit_count.coeff_u[stat_frame_type] += (stream->bitcnt - bit_start);
          }
          else{
            memset(coeff,0,sizeC/2*sizeC/2*sizeof(int16_t));
          }

           
          coeff = coeff_v + index*sizeC/2*sizeC/2;
          if (cbp.v){
            bit_start = stream->bitcnt;
            read_coeff(stream,coeff,sizeC/2,coeff_block_type|1);
            decoder_info->bit_count.coeff_v[stat_frame_type] += (stream->bitcnt - bit_start);
          }
          else{
            memset(coeff,0,sizeC/2*sizeC/2*sizeof(int16_t));
          }
        }
        block_info->cbp.y = 1;  
        block_info->cbp.u = 1;
        block_info->cbp.v = 1;
      }
      else{
        int index;
        int16_t *coeff;

         
        for (index=0;index<4;index++){
          bit_start = stream->bitcnt;
          cbp.y = get_flc(1, stream);
          decoder_info->bit_count.cbp[stat_frame_type] += (stream->bitcnt - bit_start);

           
          coeff = coeff_y + index*sizeY/2*sizeY/2;
          if (cbp.y){
            bit_start = stream->bitcnt;
            read_coeff(stream,coeff,sizeY/2,coeff_block_type|0);
            decoder_info->bit_count.coeff_y[stat_frame_type] += (stream->bitcnt - bit_start);
          }
          else{
            memset(coeff,0,sizeY/2*sizeY/2*sizeof(int16_t));
          }
        }

        bit_start = stream->bitcnt;
        if (decoder_info->subsample != 400) {
          int tmp;
          tmp = get_vlc(13, stream);
          cbp.u = tmp & 1;
          cbp.v = (tmp >> 1) & 1;
        } else
          cbp.u = cbp.v = 0;
        decoder_info->bit_count.cbp[stat_frame_type] += (stream->bitcnt - bit_start);
        if (cbp.u){
          bit_start = stream->bitcnt;
          read_coeff(stream,coeff_u,sizeC,coeff_block_type|1);
          decoder_info->bit_count.coeff_u[stat_frame_type] += (stream->bitcnt - bit_start);
        }
        else
          memset(coeff_u,0,sizeC*sizeC*sizeof(int16_t));
        if (cbp.v){
          bit_start = stream->bitcnt;
          read_coeff(stream,coeff_v,sizeC,coeff_block_type|1);
          decoder_info->bit_count.coeff_v[stat_frame_type] += (stream->bitcnt - bit_start);
        }
        else
          memset(coeff_v,0,sizeC*sizeC*sizeof(int16_t));

        block_info->cbp.y = 1;  
        block_info->cbp.u = 1;
        block_info->cbp.v = 1;
      }  
    }  
  }  
  else{
    tb_split = 0;
    block_info->cbp.y = 0;
    block_info->cbp.u = 0;
    block_info->cbp.v = 0;
  }

   
  if (mode==MODE_BIPRED){
    memcpy(block_info->block_param.mv_arr0,mv_arr0,4*sizeof(mv_t));  
    memcpy(block_info->block_param.mv_arr1,mv_arr1,4*sizeof(mv_t));  
  }
  else if(mode==MODE_SKIP){
    memcpy(block_info->block_param.mv_arr0,mv_arr0,4*sizeof(mv_t));  
    memcpy(block_info->block_param.mv_arr1,mv_arr1,4*sizeof(mv_t));  
  }
  else if(mode==MODE_MERGE){
    memcpy(block_info->block_param.mv_arr0,mv_arr0,4*sizeof(mv_t));  
    memcpy(block_info->block_param.mv_arr1,mv_arr1,4*sizeof(mv_t));  
  }
  else{
    memcpy(block_info->block_param.mv_arr0,mv_arr,4*sizeof(mv_t));  
    memcpy(block_info->block_param.mv_arr1,mv_arr,4*sizeof(mv_t));  
  }
  block_info->block_param.mode = mode;
  block_info->block_param.tb_split = tb_split;

  int bwidth = min(size,width - xpos);
  int bheight = min(size,height - ypos);

   
  decoder_info->bit_count.mode[stat_frame_type][mode] += (bwidth/MIN_BLOCK_SIZE * bheight/MIN_BLOCK_SIZE);
  decoder_info->bit_count.size[stat_frame_type][log2i(size)-3] += (bwidth/MIN_BLOCK_SIZE * bheight/MIN_BLOCK_SIZE);
  decoder_info->bit_count.size_and_mode[stat_frame_type][log2i(size)-3][mode] += (bwidth/MIN_BLOCK_SIZE * bheight/MIN_BLOCK_SIZE);
  return 0;
}