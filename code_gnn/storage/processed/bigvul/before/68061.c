struct condition * compiler_concat_conditions(struct condition *a, u_int16 op, struct condition *b)
{
   struct condition *head = a;
   
    
   while(a->next != NULL)
      a = a->next;
   
    
   a->op = op;

    
   a->next = b;
   
    
   return head;
}
