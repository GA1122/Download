static int codeCompare(
  Parse *pParse,     
  Expr *pLeft,       
  Expr *pRight,      
  int opcode,        
  int in1, int in2,  
  int dest,          
  int jumpIfNull     
){
  int p5;
  int addr;
  CollSeq *p4;

  p4 = sqlite3BinaryCompareCollSeq(pParse, pLeft, pRight);
  p5 = binaryCompareP5(pLeft, pRight, jumpIfNull);
  addr = sqlite3VdbeAddOp4(pParse->pVdbe, opcode, in2, dest, in1,
                           (void*)p4, P4_COLLSEQ);
  sqlite3VdbeChangeP5(pParse->pVdbe, (u8)p5);
  return addr;
}
