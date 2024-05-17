gsicc_init_hash_cs(cmm_profile_t *picc_profile, gs_gstate *pgs)
{
    if ( !(picc_profile->hash_is_valid) ) {
        gsicc_get_icc_buff_hash(picc_profile->buffer, &(picc_profile->hashcode),
                                picc_profile->buffer_size);
        picc_profile->hash_is_valid = true;
    }
    gsicc_set_default_cs_value(picc_profile, pgs);
}
