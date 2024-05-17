 static int dtls1_retrieve_buffered_fragment(SSL *s, int *ok)
 {
      
     pitem *item;
     hm_fragment *frag;
      int al;
  
      *ok = 0;
    item = pqueue_peek(s->d1->buffered_messages);
    if (item == NULL)
        return 0;
  
    frag = (hm_fragment *)item->data;
//     do {
//         item = pqueue_peek(s->d1->buffered_messages);
//         if (item == NULL)
//             return 0;
// 
//         frag = (hm_fragment *)item->data;
// 
//         if (frag->msg_header.seq < s->d1->handshake_read_seq) {
//              
//             pqueue_pop(s->d1->buffered_messages);
//             dtls1_hm_fragment_free(frag);
//             pitem_free(item);
//             item = NULL;
//             frag = NULL;
//         }
//     } while (item == NULL);
  
       
      if (frag->reassembly != NULL)
                    frag->msg_header.frag_len);
         }