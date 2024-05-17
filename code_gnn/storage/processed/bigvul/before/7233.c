static void ps_sd_destroy(ps_mm *data, ps_sd *sd)
{
	php_uint32 slot;

	slot = ps_sd_hash(sd->key, strlen(sd->key)) & data->hash_max;

	if (data->hash[slot] == sd) {
		data->hash[slot] = sd->next;
	} else {
		ps_sd *prev;

		 
		for (prev = data->hash[slot]; prev->next != sd; prev = prev->next);
		prev->next = sd->next;
	}

	data->hash_cnt--;

	if (sd->data) {
		mm_free(data->mm, sd->data);
	}

	mm_free(data->mm, sd);
}
