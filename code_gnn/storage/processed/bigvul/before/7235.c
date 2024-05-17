static ps_sd *ps_sd_lookup(ps_mm *data, const char *key, int rw)
{
	php_uint32 hv, slot;
	ps_sd *ret, *prev;

	hv = ps_sd_hash(key, strlen(key));
	slot = hv & data->hash_max;

	for (prev = NULL, ret = data->hash[slot]; ret; prev = ret, ret = ret->next) {
		if (ret->hv == hv && !strcmp(ret->key, key)) {
			break;
		}
	}

	if (ret && rw && ret != data->hash[slot]) {
		 
		if (prev) {
			prev->next = ret->next;
		}

		ret->next = data->hash[slot];
		data->hash[slot] = ret;
	}

	ps_mm_debug(("lookup(%s): ret=%p,hv=%u,slot=%d\n", key, ret, hv, slot));

        return ret;
 }
