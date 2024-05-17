static void codeOffset(
  Vdbe *v,           
  int iOffset,       
  int iContinue      
){
  if( iOffset>0 ){
    sqlite3VdbeAddOp3(v, OP_IfPos, iOffset, iContinue, 1); VdbeCoverage(v);
    VdbeComment((v, "OFFSET"));
  }
}
