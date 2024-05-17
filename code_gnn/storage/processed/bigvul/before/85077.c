static void copy_deblock_data(decoder_info_t *decoder_info, block_info_dec_t *block_info){

  int size = block_info->block_pos.size;
  int block_posy = block_info->block_pos.ypos/MIN_PB_SIZE;
  int block_posx = block_info->block_pos.xpos/MIN_PB_SIZE;
  int block_stride = decoder_info->width/MIN_PB_SIZE;
  int block_index;
  int m,n,m0,n0,index;
  int div = size/(2*MIN_PB_SIZE);
  int bwidth =  block_info->block_pos.bwidth;
  int bheight =  block_info->block_pos.bheight;
  uint8_t tb_split = block_info->block_param.tb_split > 0;
  part_t pb_part = block_info->block_param.mode == MODE_INTER ? block_info->block_param.pb_part : PART_NONE;  

  for (m=0;m<bheight/MIN_PB_SIZE;m++){
    for (n=0;n<bwidth/MIN_PB_SIZE;n++){
      block_index = (block_posy+m)*block_stride + block_posx+n;
      m0 = div > 0 ? m/div : 0;
      n0 = div > 0 ? n/div : 0;
      index = 2*m0+n0;
      if (index > 3) printf("error: index=%4d\n",index);
      decoder_info->deblock_data[block_index].cbp = block_info->cbp;
      decoder_info->deblock_data[block_index].tb_split = tb_split;
      decoder_info->deblock_data[block_index].pb_part = pb_part;
      decoder_info->deblock_data[block_index].size = block_info->block_pos.size;

      decoder_info->deblock_data[block_index].mode = block_info->block_param.mode;
      if (decoder_info->bit_count.stat_frame_type == B_FRAME && decoder_info->interp_ref == 2 && block_info->block_param.mode == MODE_SKIP && block_info->block_param.skip_idx==0) {
        int phase = decoder_info->frame_info.phase;
        decoder_info->deblock_data[block_index].inter_pred.mv0 = decoder_info->deblock_data[block_index].inter_pred_arr[phase].mv0;
        decoder_info->deblock_data[block_index].inter_pred.mv1 = decoder_info->deblock_data[block_index].inter_pred_arr[phase].mv0;
        if (decoder_info->num_reorder_pics == 2 && phase == 1) {
          decoder_info->deblock_data[block_index].inter_pred.mv1.x *= 2;
          decoder_info->deblock_data[block_index].inter_pred.mv1.y *= 2;
        }
      }
      else {
        decoder_info->deblock_data[block_index].inter_pred.mv0 = block_info->block_param.mv_arr0[index];
        decoder_info->deblock_data[block_index].inter_pred.mv1 = block_info->block_param.mv_arr1[index];
      }
      decoder_info->deblock_data[block_index].inter_pred.ref_idx0 = block_info->block_param.ref_idx0;
      decoder_info->deblock_data[block_index].inter_pred.ref_idx1 = block_info->block_param.ref_idx1;
      decoder_info->deblock_data[block_index].inter_pred.bipred_flag = block_info->block_param.dir;
    }
  }
}