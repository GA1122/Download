NameValueParserGetData(void * d, const char * datas, int l)
{
    struct NameValueParserData * data = (struct NameValueParserData *)d;
	if(strcmp(data->curelt, "NewPortListing") == 0)
	{
		 
		data->portListing = malloc(l + 1);
		if(!data->portListing)
		{
			 
#ifdef DEBUG
			fprintf(stderr, "%s: error allocating memory",
			        "NameValueParserGetData");
#endif  
			return;
		}
		memcpy(data->portListing, datas, l);
		data->portListing[l] = '\0';
		data->portListingLength = l;
	}
	else
	{
		 
		data->cdata = datas;
		data->cdatalen = l;
	}
}
