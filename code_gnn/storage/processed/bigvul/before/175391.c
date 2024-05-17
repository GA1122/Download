long _book_maptype1_quantvals(codebook *b){
  
 int bits=_ilog(b->entries);
 int vals=b->entries>>((bits-1)*(b->dim-1)/b->dim);

 while(1){
 long acc=1;
 long acc1=1;
 int i;
 for(i=0;i<b->dim;i++){
      acc*=vals;
      acc1*=vals+1;
 }
 if(acc<=b->entries && acc1>b->entries){
 return(vals);
 }else{
 if(acc>b->entries){
        vals--;
 }else{
        vals++;
 }
 }
 }
}
