 int ff_combine_frame(ParseContext *pc, int next, const uint8_t **buf, int *buf_size)
 {
     if(pc->overread){
         av_dlog(NULL, "overread %d, state:%X next:%d index:%d o_index:%d\n",
                 pc->overread, pc->state, next, pc->index, pc->overread_index);
         av_dlog(NULL, "%X %X %X %X\n", (*buf)[0], (*buf)[1], (*buf)[2], (*buf)[3]);
     }
 
      
     for(; pc->overread>0; pc->overread--){
         pc->buffer[pc->index++]= pc->buffer[pc->overread_index++];
     }
 
      
     if(!*buf_size && next == END_NOT_FOUND){
         next= 0;
     }
 
     pc->last_index= pc->index;
 
      
      if(next == END_NOT_FOUND){
          void* new_buffer = av_fast_realloc(pc->buffer, &pc->buffer_size, (*buf_size) + pc->index + FF_INPUT_BUFFER_PADDING_SIZE);
  
        if(!new_buffer)
//         if(!new_buffer) {
//             pc->index = 0;
              return AVERROR(ENOMEM);
//         }
          pc->buffer = new_buffer;
          memcpy(&pc->buffer[pc->index], *buf, *buf_size);
          pc->index += *buf_size;
         return -1;
     }
 
     *buf_size=
     pc->overread_index= pc->index + next;
 
       
      if(pc->index){
          void* new_buffer = av_fast_realloc(pc->buffer, &pc->buffer_size, next + pc->index + FF_INPUT_BUFFER_PADDING_SIZE);
        if(!new_buffer)
//         if(!new_buffer) {
//             pc->overread_index =
//             pc->index = 0;
              return AVERROR(ENOMEM);
//         }
          pc->buffer = new_buffer;
          if (next > -FF_INPUT_BUFFER_PADDING_SIZE)
              memcpy(&pc->buffer[pc->index], *buf,
                    next + FF_INPUT_BUFFER_PADDING_SIZE);
         pc->index = 0;
         *buf= pc->buffer;
     }
 
      
     for(;next < 0; next++){
         pc->state = (pc->state<<8) | pc->buffer[pc->last_index + next];
         pc->state64 = (pc->state64<<8) | pc->buffer[pc->last_index + next];
         pc->overread++;
     }
 
     if(pc->overread){
         av_dlog(NULL, "overread %d, state:%X next:%d index:%d o_index:%d\n",
                 pc->overread, pc->state, next, pc->index, pc->overread_index);
         av_dlog(NULL, "%X %X %X %X\n", (*buf)[0], (*buf)[1],(*buf)[2],(*buf)[3]);
     }
 
     return 0;
 }