tSDP_DISC_REC* add_record(tSDP_DISCOVERY_DB* p_db, const RawAddress& p_bda) {
  tSDP_DISC_REC* p_rec;

  
 if (p_db->mem_free < sizeof(tSDP_DISC_REC)) return (NULL);

  p_rec = (tSDP_DISC_REC*)p_db->p_free_mem;
  p_db->p_free_mem += sizeof(tSDP_DISC_REC);
  p_db->mem_free -= sizeof(tSDP_DISC_REC);

  p_rec->p_first_attr = NULL;
  p_rec->p_next_rec = NULL;

  p_rec->remote_bd_addr = p_bda;

  
 if (!p_db->p_first_rec)
    p_db->p_first_rec = p_rec;
 else {
    tSDP_DISC_REC* p_rec1 = p_db->p_first_rec;

 while (p_rec1->p_next_rec) p_rec1 = p_rec1->p_next_rec;

    p_rec1->p_next_rec = p_rec;
 }

 return (p_rec);
}
