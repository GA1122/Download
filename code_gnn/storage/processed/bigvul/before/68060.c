struct block * compiler_add_instr(struct instruction *ins, struct block *blk)
{
   struct block *bl;

   SAFE_CALLOC(bl, 1, sizeof(struct block));

    
   bl->type = BLK_INSTR;
   bl->un.ins = ins;

    
   bl->next = blk;

   return bl;
}
