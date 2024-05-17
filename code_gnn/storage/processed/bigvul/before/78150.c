static const char *tag2str(unsigned int tag)
{
	static const char *tags[] = {
		"EOC", "BOOLEAN", "INTEGER", "BIT STRING", "OCTET STRING",	 
		"NULL", "OBJECT IDENTIFIER", "OBJECT DESCRIPTOR", "EXTERNAL", "REAL",	 
		"ENUMERATED", "Universal 11", "UTF8String", "Universal 13",	 
		"Universal 14", "Universal 15", "SEQUENCE", "SET",	 
		"NumericString", "PrintableString", "T61String",	 
		"VideotexString", "IA5String", "UTCTIME", "GENERALIZEDTIME",	 
		"GraphicString", "VisibleString", "GeneralString",	 
		"UniversalString", "Universal 29", "BMPString"	 
	};

	if (tag > 30)
		return "(unknown)";
	return tags[tag];
}
