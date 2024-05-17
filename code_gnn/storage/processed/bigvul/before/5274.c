kwsprep (kwset_t kwset)
{
  char const *trans = kwset->trans;
  int i;
  unsigned char deltabuf[NCHAR];
  unsigned char *delta = trans ? deltabuf : kwset->delta;

   
  memset (delta, MIN (kwset->mind, UCHAR_MAX), sizeof deltabuf);

   
  struct trie *curr, *last;
  for (curr = last = kwset->trie; curr; curr = curr->next)
    {
       
      enqueue (curr->links, &last);

      curr->shift = kwset->mind;
      curr->maxshift = kwset->mind;

       
      treedelta (curr->links, curr->depth, delta);

       
      treefails (curr->links, curr->fail, kwset->trie);

       
      struct trie *fail;
      for (fail = curr->fail; fail; fail = fail->fail)
        {
           
          if (!hasevery (fail->links, curr->links))
            if (curr->depth - fail->depth < fail->shift)
              fail->shift = curr->depth - fail->depth;

           
          if (curr->accepting && fail->maxshift > curr->depth - fail->depth)
            fail->maxshift = curr->depth - fail->depth;
        }
    }

   
  for (curr = kwset->trie->next; curr; curr = curr->next)
    {
      if (curr->maxshift > curr->parent->maxshift)
        curr->maxshift = curr->parent->maxshift;
      if (curr->shift > curr->maxshift)
        curr->shift = curr->maxshift;
    }

   
  struct trie *nextbuf[NCHAR];
  struct trie **next = trans ? nextbuf : kwset->next;
  memset (next, 0, sizeof nextbuf);
  treenext (kwset->trie->links, next);
  if (trans)
    for (i = 0; i < NCHAR; ++i)
      kwset->next[i] = next[U(trans[i])];

   
  if (kwset->words == 1)
    {
       
      kwset->target = obstack_alloc (&kwset->obstack, kwset->mind);
      for (i = kwset->mind - 1, curr = kwset->trie; i >= 0; --i)
        {
          kwset->target[i] = curr->links->label;
          curr = curr->next;
        }
       
      if (kwset->mind > 1)
        {
          kwset->shift
            = obstack_alloc (&kwset->obstack,
                             sizeof *kwset->shift * (kwset->mind - 1));
          for (i = 0, curr = kwset->trie->next; i < kwset->mind - 1; ++i)
            {
              kwset->shift[i] = curr->shift;
              curr = curr->next;
            }
        }

      char gc1 = tr (trans, kwset->target[kwset->mind - 1]);

       
      int gc1help = -1;
      if (trans)
        {
          char const *equiv1 = memchr (trans, gc1, NCHAR);
          char const *equiv2 = memchr (equiv1 + 1, gc1,
                                       trans + NCHAR - (equiv1 + 1));
          if (equiv2)
            gc1help = (memchr (equiv2 + 1, gc1, trans + NCHAR - (equiv2 + 1))
                       ? NCHAR
                       : U(gc1) ^ (equiv1 - trans) ^ (equiv2 - trans));
        }

      kwset->gc1 = gc1;
      kwset->gc1help = gc1help;
      if (kwset->mind > 1)
        kwset->gc2 = tr (trans, kwset->target[kwset->mind - 2]);
    }

   
  if (trans)
    for (i = 0; i < NCHAR; ++i)
      kwset->delta[i] = delta[U(trans[i])];
}
