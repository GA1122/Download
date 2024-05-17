kwsprep (kwset_t kws)
{
  struct kwset *kwset;
  int i;
  struct trie *curr;
  char const *trans;
  unsigned char delta[NCHAR];

  kwset = (struct kwset *) kws;

   
  memset(delta, kwset->mind < UCHAR_MAX ? kwset->mind : UCHAR_MAX, NCHAR);

   
  if (kwset->words == 1 && kwset->trans == NULL)
    {
      char c;

       
      kwset->target = obstack_alloc(&kwset->obstack, kwset->mind);
      if (!kwset->target)
        return _("memory exhausted");
      for (i = kwset->mind - 1, curr = kwset->trie; i >= 0; --i)
        {
          kwset->target[i] = curr->links->label;
          curr = curr->links->trie;
        }
       
      for (i = 0; i < kwset->mind; ++i)
        delta[U(kwset->target[i])] = kwset->mind - (i + 1);
       
      c = kwset->target[kwset->mind - 1];
      for (i = kwset->mind - 2; i >= 0; --i)
        if (kwset->target[i] == c)
          break;
      kwset->mind2 = kwset->mind - (i + 1);
    }
  else
    {
      struct trie *fail;
      struct trie *last, *next[NCHAR];

       
      for (curr = last = kwset->trie; curr; curr = curr->next)
        {
           
          enqueue(curr->links, &last);

          curr->shift = kwset->mind;
          curr->maxshift = kwset->mind;

           
          treedelta(curr->links, curr->depth, delta);

           
          treefails(curr->links, curr->fail, kwset->trie);

           
          for (fail = curr->fail; fail; fail = fail->fail)
            {
               
              if (!hasevery(fail->links, curr->links))
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

       
      for (i = 0; i < NCHAR; ++i)
        next[i] = NULL;
      treenext(kwset->trie->links, next);

      if ((trans = kwset->trans) != NULL)
        for (i = 0; i < NCHAR; ++i)
          kwset->next[i] = next[U(trans[i])];
      else
        memcpy(kwset->next, next, NCHAR * sizeof(struct trie *));
    }

   
  if ((trans = kwset->trans) != NULL)
    for (i = 0; i < NCHAR; ++i)
      kwset->delta[i] = delta[U(trans[i])];
  else
    memcpy(kwset->delta, delta, NCHAR);

  return NULL;
}
