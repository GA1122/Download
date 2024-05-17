static uint32_t uuid_to_id(const effect_uuid_t * uuid)
{
 size_t i;
 for (i = 0; i < NUM_ID; i++)
 if (memcmp(uuid, uuid_to_id_table[i], sizeof(*uuid)) == 0)
 break;

 return i;
}
