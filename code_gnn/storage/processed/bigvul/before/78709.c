static int get_name_from_EF_DatiPersonali(unsigned char *EFdata,
	char name[], int name_len)
{
	 

	const unsigned int EF_personaldata_maxlen = 400;
	const unsigned int tlv_length_size = 6;
	char *file = (char*)&EFdata[tlv_length_size];
	int file_size = hextoint((char*)EFdata, tlv_length_size);

	enum {
		f_issuer_code = 0,
		f_issuing_date,
		f_expiry_date,
		f_last_name,
		f_first_name,
		f_birth_date,
		f_sex,
		f_height,
		f_codice_fiscale,
		f_citizenship_code,
		f_birth_township_code,
		f_birth_country,
		f_birth_certificate,
		f_residence_township_code,
		f_residence_address,
		f_expat_notes
	};

	 
	struct {
		int len;
		char value[256];
	} fields[f_first_name+1];
	int i=0;  
	int f;  

	if(file_size < 0)
		return -1;

	 
	if(file_size > (int)EF_personaldata_maxlen - (int)tlv_length_size)
		file_size = EF_personaldata_maxlen - tlv_length_size;


	memset(fields, 0, sizeof(fields));

	for(f=0; f<f_first_name+1; f++) {
		int field_size;
		 
		if(i > file_size)
			return -1;

		field_size = hextoint((char*) &file[i], 2);
		if((field_size < 0) || (field_size+i > file_size))
			return -1;

		i += 2;

		if(field_size >= (int)sizeof(fields[f].value))
			return -1;

		fields[f].len = field_size;
		strncpy(fields[f].value, &file[i], field_size);
		fields[f].value[field_size] = '\0';
		i += field_size;
	}

	if (fields[f_first_name].len + fields[f_last_name].len + 1 >= name_len)
		return -1;

	 
	snprintf(name, name_len, "%.*s %.*s",
		fields[f_first_name].len, fields[f_first_name].value,
		fields[f_last_name].len, fields[f_last_name].value);
	return 0;
}
