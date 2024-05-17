static void unfold_ifblk(struct block **blk)
{
   struct ifblock *ifblk;
   struct unfold_elm *ue;
   u_int32 a = vlabel++; 
   u_int32 b = vlabel++; 
   u_int32 c = vlabel++; 

    

    
   ef_debug(1, "#"); 
   
    
   ifblk = (*blk)->un.ifb;
  
    
   unfold_conds(ifblk->conds, a, b);
   
    
   SAFE_CALLOC(ue, 1, sizeof(struct unfold_elm));
   ue->label = a;
   TAILQ_INSERT_TAIL(&unfolded_tree, ue, next);

    
   if (ifblk->blk != NULL) {
       
      unfold_blk(&ifblk->blk);
   }

    
   if (ifblk->elseblk != NULL) {
      SAFE_CALLOC(ue, 1, sizeof(struct unfold_elm));
      ue->fop.opcode = FOP_JMP;
      ue->fop.op.jmp = c;
      TAILQ_INSERT_TAIL(&unfolded_tree, ue, next);
   }
   
    
   SAFE_CALLOC(ue, 1, sizeof(struct unfold_elm));
   ue->label = b;
   TAILQ_INSERT_TAIL(&unfolded_tree, ue, next);
   
    
   if (ifblk->elseblk != NULL) {
      unfold_blk(&ifblk->elseblk);
       
      SAFE_CALLOC(ue, 1, sizeof(struct unfold_elm));
      ue->label = c;
      TAILQ_INSERT_TAIL(&unfolded_tree, ue, next);
   }

}
