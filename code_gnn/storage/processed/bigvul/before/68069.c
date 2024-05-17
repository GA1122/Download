static void unfold_conds(struct condition *cnd, u_int32 a, u_int32 b)
{
   struct unfold_elm *ue = NULL;
 
   do {
   
       
      ef_debug(1, "?"); 
   
       
      SAFE_CALLOC(ue, 1, sizeof(struct unfold_elm));
      memcpy(&ue->fop, &cnd->fop, sizeof(struct filter_op));
      TAILQ_INSERT_TAIL(&unfolded_tree, ue, next);
      
       
      SAFE_CALLOC(ue, 1, sizeof(struct unfold_elm));
      
      if (cnd->op == COND_OR) {
         ue->fop.opcode = FOP_JTRUE;
         ue->fop.op.jmp = a;
      } else {
          
         ue->fop.opcode = FOP_JFALSE;
         ue->fop.op.jmp = b;
      }
      
      TAILQ_INSERT_TAIL(&unfolded_tree, ue, next);
      
   } while ((cnd = cnd->next));
   
}
