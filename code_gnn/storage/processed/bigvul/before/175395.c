int _ilog(unsigned int v){
 int ret=0;
 while(v){
    ret++;
    v>>=1;
 }
 return(ret);

 }
