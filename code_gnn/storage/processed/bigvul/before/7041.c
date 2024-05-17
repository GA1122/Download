  Sort( PProfileList  list )
  {
    PProfile  *old, current, next;


     
    current = *list;
    while ( current )
    {
      current->X       = *current->offset;
      current->offset += current->flags & Flow_Up ? 1 : -1;
      current->height--;
      current = current->link;
    }

     
    old     = list;
    current = *old;

    if ( !current )
      return;

    next = current->link;

    while ( next )
    {
      if ( current->X <= next->X )
      {
        old     = &current->link;
        current = *old;

        if ( !current )
          return;
      }
      else
      {
        *old          = next;
        current->link = next->link;
        next->link    = current;

        old     = list;
        current = *old;
      }

      next = current->link;
    }
  }