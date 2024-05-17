ext4_xattr_set_entry(struct ext4_xattr_info *i, struct ext4_xattr_search *s)
{
	struct ext4_xattr_entry *last;
	size_t free, min_offs = s->end - s->base, name_len = strlen(i->name);

	 
	last = s->first;
	for (; !IS_LAST_ENTRY(last); last = EXT4_XATTR_NEXT(last)) {
		if (!last->e_value_block && last->e_value_size) {
			size_t offs = le16_to_cpu(last->e_value_offs);
			if (offs < min_offs)
				min_offs = offs;
		}
	}
	free = min_offs - ((void *)last - s->base) - sizeof(__u32);
	if (!s->not_found) {
		if (!s->here->e_value_block && s->here->e_value_size) {
			size_t size = le32_to_cpu(s->here->e_value_size);
			free += EXT4_XATTR_SIZE(size);
		}
		free += EXT4_XATTR_LEN(name_len);
	}
	if (i->value) {
		if (free < EXT4_XATTR_LEN(name_len) +
			   EXT4_XATTR_SIZE(i->value_len))
			return -ENOSPC;
	}

	if (i->value && s->not_found) {
		 
		size_t size = EXT4_XATTR_LEN(name_len);
		size_t rest = (void *)last - (void *)s->here + sizeof(__u32);
		memmove((void *)s->here + size, s->here, rest);
		memset(s->here, 0, size);
		s->here->e_name_index = i->name_index;
		s->here->e_name_len = name_len;
		memcpy(s->here->e_name, i->name, name_len);
	} else {
		if (!s->here->e_value_block && s->here->e_value_size) {
			void *first_val = s->base + min_offs;
			size_t offs = le16_to_cpu(s->here->e_value_offs);
			void *val = s->base + offs;
			size_t size = EXT4_XATTR_SIZE(
				le32_to_cpu(s->here->e_value_size));

			if (i->value && size == EXT4_XATTR_SIZE(i->value_len)) {
				 
				s->here->e_value_size =
					cpu_to_le32(i->value_len);
				if (i->value == EXT4_ZERO_XATTR_VALUE) {
					memset(val, 0, size);
				} else {
					 
					memset(val + size - EXT4_XATTR_PAD, 0,
					       EXT4_XATTR_PAD);
					memcpy(val, i->value, i->value_len);
				}
				return 0;
			}

			 
			memmove(first_val + size, first_val, val - first_val);
			memset(first_val, 0, size);
			s->here->e_value_size = 0;
			s->here->e_value_offs = 0;
			min_offs += size;

			 
			last = s->first;
			while (!IS_LAST_ENTRY(last)) {
				size_t o = le16_to_cpu(last->e_value_offs);
				if (!last->e_value_block &&
				    last->e_value_size && o < offs)
					last->e_value_offs =
						cpu_to_le16(o + size);
				last = EXT4_XATTR_NEXT(last);
			}
		}
		if (!i->value) {
			 
			size_t size = EXT4_XATTR_LEN(name_len);
			last = ENTRY((void *)last - size);
			memmove(s->here, (void *)s->here + size,
				(void *)last - (void *)s->here + sizeof(__u32));
			memset(last, 0, size);
		}
	}

	if (i->value) {
		 
		s->here->e_value_size = cpu_to_le32(i->value_len);
		if (i->value_len) {
			size_t size = EXT4_XATTR_SIZE(i->value_len);
			void *val = s->base + min_offs - size;
			s->here->e_value_offs = cpu_to_le16(min_offs - size);
			if (i->value == EXT4_ZERO_XATTR_VALUE) {
				memset(val, 0, size);
			} else {
				 
				memset(val + size - EXT4_XATTR_PAD, 0,
				       EXT4_XATTR_PAD);
				memcpy(val, i->value, i->value_len);
			}
		}
	}
	return 0;
}
