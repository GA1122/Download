 ParseNameValue(const char * buffer, int bufsize,
                 struct NameValueParserData * data)
  {
  	struct xmlparser parser;
	data->l_head = NULL;
	data->portListing = NULL;
	data->portListingLength = 0;
// 	memset(data, 0, sizeof(struct NameValueParserData));
  	 
  	parser.xmlstart = buffer;
  	parser.xmlsize = bufsize;
 	parser.data = data;
 	parser.starteltfunc = NameValueParserStartElt;
 	parser.endeltfunc = NameValueParserEndElt;
 	parser.datafunc = NameValueParserGetData;
 	parser.attfunc = 0;
 	parsexml(&parser);
 }