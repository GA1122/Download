static void iriap_getvaluebyclass_confirm(struct iriap_cb *self,
					  struct sk_buff *skb)
{
	struct ias_value *value;
	int charset;
	__u32 value_len;
	__u32 tmp_cpu32;
	__u16 obj_id;
	__u16 len;
	__u8  type;
	__u8 *fp;
	int n;

	IRDA_ASSERT(self != NULL, return;);
	IRDA_ASSERT(self->magic == IAS_MAGIC, return;);
	IRDA_ASSERT(skb != NULL, return;);

	 
	fp = skb->data;
	n = 2;

	 
	len = get_unaligned_be16(fp + n);
	n += 2;

	IRDA_DEBUG(4, "%s(), len=%d\n", __func__, len);

	 
	obj_id = get_unaligned_be16(fp + n);
	n += 2;

	type = fp[n++];
	IRDA_DEBUG(4, "%s(), Value type = %d\n", __func__, type);

	switch (type) {
	case IAS_INTEGER:
		memcpy(&tmp_cpu32, fp+n, 4); n += 4;
		be32_to_cpus(&tmp_cpu32);
		value = irias_new_integer_value(tmp_cpu32);

		 
		IRDA_DEBUG(4, "%s(), lsap=%d\n", __func__, value->t.integer);
		break;
	case IAS_STRING:
		charset = fp[n++];

		switch (charset) {
		case CS_ASCII:
			break;
 
 
 
 
 
 
 
 
 
 
		default:
			IRDA_DEBUG(0, "%s(), charset %s, not supported\n",
				   __func__, ias_charset_types[charset]);

			 
			iriap_disconnect_request(self);
			return;
			 
		}
		value_len = fp[n++];
		IRDA_DEBUG(4, "%s(), strlen=%d\n", __func__, value_len);

		 
		if (n + value_len < skb->len)
			fp[n + value_len] = 0x00;
		IRDA_DEBUG(4, "Got string %s\n", fp+n);

		 
		value = irias_new_string_value(fp+n);
		break;
	case IAS_OCT_SEQ:
		value_len = get_unaligned_be16(fp + n);
		n += 2;

		 
		value = irias_new_octseq_value(fp+n, value_len);
		break;
	default:
		value = irias_new_missing_value();
		break;
	}

	 
	iriap_disconnect_request(self);

	 
	if (self->confirm)
		self->confirm(IAS_SUCCESS, obj_id, value, self->priv);
	else {
		IRDA_DEBUG(0, "%s(), missing handler!\n", __func__);
		irias_delete_value(value);
	}
}
