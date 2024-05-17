static int _determine_node_bytes(long used, int leafwidth){

  
 if(used<2)
 return 4;

 if(leafwidth==3)leafwidth=4;
 if(_ilog(3*used-6)+1 <= leafwidth*4)
 return leafwidth/2?leafwidth/2:1;
 return leafwidth;
}
