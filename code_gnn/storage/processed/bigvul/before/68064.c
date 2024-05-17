struct ifblock * compiler_create_ifelseblock(struct condition *conds, struct block *blk, struct block *elseblk)
{
   struct ifblock *ifblk;

   SAFE_CALLOC(ifblk, 1, sizeof(struct ifblock));
   
    
   ifblk->conds = conds;
   ifblk->blk = blk;
   ifblk->elseblk = elseblk;

   return ifblk;
}
