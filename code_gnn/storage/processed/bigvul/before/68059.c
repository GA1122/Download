struct block * compiler_add_ifblk(struct ifblock *ifb, struct block *blk)
{
   struct block *bl;

   SAFE_CALLOC(bl, 1, sizeof(struct block));

    
   bl->type = BLK_IFBLK;
   bl->un.ifb = ifb;

    
   bl->next = blk;

   return bl;
}
