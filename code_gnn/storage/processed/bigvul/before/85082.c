static int decode_super_mode(decoder_info_t *decoder_info, int size, int decode_this_size){
  stream_t *stream = decoder_info->stream;
  block_context_t *block_context = decoder_info->block_context;

  frame_type_t frame_type = decoder_info->frame_info.frame_type;
  int split_flag = 0;
  int mode = MODE_SKIP;
  int stat_mode = STAT_SKIP;
  int num_ref=0,code,maxbit;
  int idx = log2i(size)-3;

  decoder_info->mode = MODE_SKIP;  

  if (frame_type==I_FRAME){
    decoder_info->mode = MODE_INTRA;
    if (size > MIN_BLOCK_SIZE && decode_this_size)
      split_flag = get_flc(1, stream);
    else
      split_flag = !decode_this_size;
    return split_flag;
  }

  if (!decode_this_size) {
    split_flag = !get_flc(1, stream);
    return split_flag;
  }

  if (size > MAX_TR_SIZE) {
    split_flag = !get_flc(1, stream);
    if (!split_flag)  decoder_info->mode = MODE_SKIP;
    return split_flag;
  }


  num_ref = decoder_info->frame_info.num_ref;
  int bipred_possible_flag = num_ref > 1 && decoder_info->bipred;
  int split_possible_flag = size > MIN_BLOCK_SIZE;
  maxbit = 2 + num_ref + split_possible_flag + bipred_possible_flag;

  int interp_ref = decoder_info->frame_info.interp_ref;

  if (interp_ref > 2) {
    maxbit -= 1;  
  }

  code = get_vlc(10 + maxbit, stream);

  if (interp_ref) {

    if ((block_context->index == 2 || block_context->index>3) && size>MIN_BLOCK_SIZE){
       
      if (code<3)
        code = (code + 1) % 3;
    }

    if (split_possible_flag && code==1) {
       
      split_flag = 1;
      decoder_info->bit_count.super_mode_stat[decoder_info->bit_count.stat_frame_type][idx][STAT_SPLIT] += 1;
      return split_flag;
    }

    if (!split_possible_flag && code > 0) {
       
      code += 1;
    }

    if (!bipred_possible_flag && code >= 3) {
       
      code += 1;
    }

    if (code == 0) {
      mode = MODE_SKIP;
      stat_mode = STAT_SKIP;
    } else if (code == 2) {
      mode = MODE_MERGE;
      stat_mode = STAT_MERGE;
    } else if (code == 3) {
      mode = MODE_BIPRED;
      stat_mode = STAT_BIPRED;
    } else if (code == 4) {
      mode = MODE_INTRA;
      stat_mode = STAT_INTRA;
    } else if (code == 4 + num_ref) {
      mode = MODE_INTER;
      decoder_info->ref_idx = 0;
      stat_mode = STAT_REF_IDX0;
    }
    else{
      mode = MODE_INTER;
      decoder_info->ref_idx = code - 4;
      stat_mode = STAT_REF_IDX1 + decoder_info->ref_idx-1;
    }
    decoder_info->mode = mode;


  } else {
    if ((block_context->index == 2 || block_context->index>3) && size>MIN_BLOCK_SIZE){
       
      if (code<4)
        code = (code + 1) % 4;
    }

    if (split_possible_flag && code==1) {
       
      split_flag = 1;
      decoder_info->bit_count.super_mode_stat[decoder_info->bit_count.stat_frame_type][idx][STAT_SPLIT] += 1;
      return split_flag;
    }

    if (!split_possible_flag && code > 0) {
       
      code += 1;
    }

    if (!bipred_possible_flag && code >= 4) {
       
      code += 1;
    }

    if (code == 0) {
      mode = MODE_SKIP;
      stat_mode = STAT_SKIP;
    }
    else if (code == 2) {
      mode = MODE_INTER;
      decoder_info->ref_idx = 0;
      stat_mode = STAT_REF_IDX0;
    }
    else if (code == 3) {
      mode = MODE_MERGE;
      stat_mode = STAT_MERGE;
    }
    else if (code == 4) {
      mode = MODE_BIPRED;
      stat_mode = STAT_BIPRED;
    }
    else if (code == 5) {
      mode = MODE_INTRA;
      stat_mode = STAT_INTRA;
    }
    else{
      mode = MODE_INTER;
      decoder_info->ref_idx = code - 5;
      stat_mode = STAT_REF_IDX1 + decoder_info->ref_idx - 1;
    }
    decoder_info->mode = mode;

  }
  decoder_info->bit_count.super_mode_stat[decoder_info->bit_count.stat_frame_type][idx][stat_mode] += 1;

  return split_flag;
}