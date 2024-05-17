static const char *parse_number(cJSON *item,const char *num)
{
	double n=0,sign=1,scale=0;int subscale=0,signsubscale=1;

	if (*num=='-') sign=-1,num++;	 
	if (*num=='0') num++;			 
	if (*num>='1' && *num<='9')	do	n=(n*10.0)+(*num++ -'0');	while (*num>='0' && *num<='9');	 
	if (*num=='.' && num[1]>='0' && num[1]<='9') {num++;		do	n=(n*10.0)+(*num++ -'0'),scale--; while (*num>='0' && *num<='9');}	 
	if (*num=='e' || *num=='E')		 
	{	num++;if (*num=='+') num++;	else if (*num=='-') signsubscale=-1,num++;		 
		while (*num>='0' && *num<='9') subscale=(subscale*10)+(*num++ - '0');	 
	}

	n=sign*n*pow(10.0,(scale+subscale*signsubscale));	 
	
	item->valuedouble=n;
	item->valueint=(int)n;
	item->type=cJSON_Number;
	return num;
}
