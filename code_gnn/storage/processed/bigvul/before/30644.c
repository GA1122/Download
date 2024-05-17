static int irda_extract_ias_value(struct irda_ias_set *ias_opt,
				  struct ias_value *ias_value)
{
	 
	switch (ias_value->type) {
	case IAS_INTEGER:
		 
		ias_opt->attribute.irda_attrib_int = ias_value->t.integer;
		break;
	case IAS_OCT_SEQ:
		 
		ias_opt->attribute.irda_attrib_octet_seq.len = ias_value->len;
		 
		memcpy(ias_opt->attribute.irda_attrib_octet_seq.octet_seq,
		       ias_value->t.oct_seq, ias_value->len);
		break;
	case IAS_STRING:
		 
		ias_opt->attribute.irda_attrib_string.len = ias_value->len;
		ias_opt->attribute.irda_attrib_string.charset = ias_value->charset;
		 
		memcpy(ias_opt->attribute.irda_attrib_string.string,
		       ias_value->t.string, ias_value->len);
		 
		ias_opt->attribute.irda_attrib_string.string[ias_value->len] = '\0';
		break;
	case IAS_MISSING:
	default :
		return -EINVAL;
	}

	 
	ias_opt->irda_attrib_type = ias_value->type;

	return 0;
}
