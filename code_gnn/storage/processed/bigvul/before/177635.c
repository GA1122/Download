 int BlockSizeLog2Min1() const {
 switch (block_size_) {
 case 16:
 return 3;
 case 8:
 return 2;
 default:
 return 0;
 }
 }
