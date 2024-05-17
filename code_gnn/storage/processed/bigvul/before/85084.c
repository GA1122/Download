void read_coeff(stream_t *stream,int16_t *coeff,int size,int type){

  int16_t scoeff[MAX_QUANT_SIZE*MAX_QUANT_SIZE];
  int i,j,levelFlag,sign,level,pos,run,tmp,code;
  int qsize = min(size,MAX_QUANT_SIZE);
  int N = qsize*qsize;
  int level_mode;
  int chroma_flag = type&1;
  int intra_flag = (type>>1)&1;
  int vlc_adaptive = intra_flag && !chroma_flag;

   
  memset(scoeff,0,N*sizeof(int16_t));
  memset(coeff,0,size*size*sizeof(int16_t));

  pos = 0;
   
  if (chroma_flag==1){
    int tmp = get_flc(1, stream);
    if (tmp){
      sign = get_flc(1, stream);
      scoeff[pos] = sign ? -1 : 1;
      pos = N;
    }
  }

   
  level_mode = 1;
  level = 1;
  while (pos < N){
    if (level_mode){
       
      while (pos < N && level > 0){
        level = get_vlc(vlc_adaptive,stream);
        if (level){
          sign = get_flc(1, stream);
        }
        else{
          sign = 1;
        }
        scoeff[pos] = sign ? -level : level;
        if (chroma_flag==0)
          vlc_adaptive = level > 3;
        pos++;
      }
    }
    if (pos >= N){
      break;
    }

     
    int eob;
    int eob_pos = chroma_flag ? 0 : 2;
    if (chroma_flag && size <= 8)
      code = get_vlc(10, stream);
    else
      code = get_vlc(6, stream);

    eob = code == eob_pos;
    if (eob) {
      break;
    }
    if (code > eob_pos) code -= 1;
    levelFlag = (code % 5) == 4;
    if (levelFlag)
      run = code / 5;
    else
      run = 4*(code/5) + code % 5;
    pos += run;

     
    if (levelFlag){
      tmp = get_vlc(0,stream);
      sign = tmp&1;
      level = (tmp>>1)+2;
    }
    else{
      level = 1;
      sign = get_flc(1, stream);
    }
    scoeff[pos] = sign ? -level : level;

    level_mode = level > 1;  
    pos++;
  }  

   
  int *zigzagptr = zigzag64;
  if (qsize==4)
    zigzagptr = zigzag16;
  else if (qsize==8)
    zigzagptr = zigzag64;
  else if (qsize==16)
    zigzagptr = zigzag256;
  for (i=0;i<qsize;i++){
    for (j=0;j<qsize;j++){
      coeff[i*qsize + j] = scoeff[zigzagptr[i*qsize + j]];
    }
  }
}