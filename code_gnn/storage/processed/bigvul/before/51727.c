wv_datetime_from_opaque(tvbuff_t *tvb, guint32 offset, guint32 data_len)
{
	char *str;
	guint16 year;
	guint8 month, day, hour, minute, second, time_zone;
	guint8 peek;

	if (data_len == 6) {  

		 
		year = tvb_get_guint8(tvb, offset) & 0x3F;  
		year <<=6;
		 
		peek = tvb_get_guint8(tvb, offset + 1);
		year += (peek >> 2);  
		month = (peek & 0x03) << 2;  
		 
		peek = tvb_get_guint8(tvb, offset + 2);
		month += (peek >> 6);  
		day = (peek & 0x3E) >> 1;  
		hour = (peek & 0x01) << 4;  
		 
		peek = tvb_get_guint8(tvb, offset + 3);
		hour += (peek >> 4);
		minute = (peek & 0x0F) << 2;  
		 
		peek = tvb_get_guint8(tvb, offset + 4);
		minute += (peek >> 6);  
		second = peek & 0x3F;  
		 
		time_zone = tvb_get_guint8(tvb, offset + 5);
		 
		str = wmem_strdup_printf(wmem_packet_scope(), "WV-CSP DateTime: "
				      "%04d-%02d-%02dT%02d:%02d:%02d%c",
				      year, month, day, hour, minute, second, time_zone);
	} else {  
		str = wmem_strdup_printf(wmem_packet_scope(), "<Error: invalid binary WV-CSP DateTime value "
				      "(%d bytes of opaque data)>", data_len);
	}
	return str;
}
