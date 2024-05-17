static int __ffs_data_got_strings(struct ffs_data *ffs,
				  char *const _data, size_t len)
{
	u32 str_count, needed_count, lang_count;
	struct usb_gadget_strings **stringtabs, *t;
	struct usb_string *strings, *s;
	const char *data = _data;

	ENTER();

	if (unlikely(get_unaligned_le32(data) != FUNCTIONFS_STRINGS_MAGIC ||
		     get_unaligned_le32(data + 4) != len))
		goto error;
	str_count  = get_unaligned_le32(data + 8);
	lang_count = get_unaligned_le32(data + 12);

	 
	if (unlikely(!str_count != !lang_count))
		goto error;

	 
	needed_count = ffs->strings_count;
	if (unlikely(str_count < needed_count))
		goto error;

	 
	if (!needed_count) {
		kfree(_data);
		return 0;
	}

	 
	{
		unsigned i = 0;
		vla_group(d);
		vla_item(d, struct usb_gadget_strings *, stringtabs,
			lang_count + 1);
		vla_item(d, struct usb_gadget_strings, stringtab, lang_count);
		vla_item(d, struct usb_string, strings,
			lang_count*(needed_count+1));

		char *vlabuf = kmalloc(vla_group_size(d), GFP_KERNEL);

		if (unlikely(!vlabuf)) {
			kfree(_data);
			return -ENOMEM;
		}

		 
		stringtabs = vla_ptr(vlabuf, d, stringtabs);
		t = vla_ptr(vlabuf, d, stringtab);
		i = lang_count;
		do {
			*stringtabs++ = t++;
		} while (--i);
		*stringtabs = NULL;

		 
		stringtabs = vla_ptr(vlabuf, d, stringtabs);
		t = vla_ptr(vlabuf, d, stringtab);
		s = vla_ptr(vlabuf, d, strings);
		strings = s;
	}

	 
	data += 16;
	len -= 16;

	do {  
		unsigned needed = needed_count;

		if (unlikely(len < 3))
			goto error_free;
		t->language = get_unaligned_le16(data);
		t->strings  = s;
		++t;

		data += 2;
		len -= 2;

		 
		do {  
			size_t length = strnlen(data, len);

			if (unlikely(length == len))
				goto error_free;

			 
			if (likely(needed)) {
				 
				s->s = data;
				--needed;
				++s;
			}

			data += length + 1;
			len -= length + 1;
		} while (--str_count);

		s->id = 0;    
		s->s = NULL;
		++s;

	} while (--lang_count);

	 
	if (unlikely(len))
		goto error_free;

	 
	ffs->stringtabs = stringtabs;
	ffs->raw_strings = _data;

	return 0;

error_free:
	kfree(stringtabs);
error:
	kfree(_data);
	return -EINVAL;
}
