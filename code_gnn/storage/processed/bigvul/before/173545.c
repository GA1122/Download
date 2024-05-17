static const effect_uuid_t * id_to_uuid(int id)
{
 if (id >= NUM_ID)
 return EFFECT_UUID_NULL;

 return uuid_to_id_table[id];
}
