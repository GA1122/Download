static void ide_cfata_identify_size(IDEState *s)
{
    uint16_t *p = (uint16_t *)s->identify_data;
    put_le16(p + 7, s->nb_sectors >> 16);   
    put_le16(p + 8, s->nb_sectors);         
    put_le16(p + 60, s->nb_sectors);        
    put_le16(p + 61, s->nb_sectors >> 16);  
}