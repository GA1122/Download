 DecodeNumberField(int len, char *str, int fmask,
 				  int *tmask, struct tm * tm, fsec_t *fsec, int *is2digits)
 {
 	char	   *cp;
 
 	 
  	if ((cp = strchr(str, '.')) != NULL)
  	{
  #ifdef HAVE_INT64_TIMESTAMP
		char		fstr[MAXDATELEN + 1];
// 		char		fstr[7];
// 		int			i;
// 
// 		cp++;
  
  		 
		strcpy(fstr, (cp + 1));
		strcpy(fstr + strlen(fstr), "000000");
		*(fstr + 6) = '\0';
// 		for (i = 0; i < 6; i++)
// 			fstr[i] = *cp != '\0' ? *cp++ : '0';
// 		fstr[i] = '\0';
  		*fsec = strtol(fstr, NULL, 10);
  #else
  		*fsec = strtod(cp, NULL);
 #endif
 		*cp = '\0';
 		len = strlen(str);
 	}
 	 
 	else if ((fmask & DTK_DATE_M) != DTK_DATE_M)
 	{
 		 
 		if (len == 8)
 		{
 			*tmask = DTK_DATE_M;
 
 			tm->tm_mday = atoi(str + 6);
 			*(str + 6) = '\0';
 			tm->tm_mon = atoi(str + 4);
 			*(str + 4) = '\0';
 			tm->tm_year = atoi(str + 0);
 
 			return DTK_DATE;
 		}
 		 
 		else if (len == 6)
 		{
 			*tmask = DTK_DATE_M;
 			tm->tm_mday = atoi(str + 4);
 			*(str + 4) = '\0';
 			tm->tm_mon = atoi(str + 2);
 			*(str + 2) = '\0';
 			tm->tm_year = atoi(str + 0);
 			*is2digits = TRUE;
 
 			return DTK_DATE;
 		}
 		 
 		else if (len == 5)
 		{
 			*tmask = DTK_DATE_M;
 			tm->tm_mday = atoi(str + 2);
 			*(str + 2) = '\0';
 			tm->tm_mon = 1;
 			tm->tm_year = atoi(str + 0);
 			*is2digits = TRUE;
 
 			return DTK_DATE;
 		}
 	}
 
 	 
 	if ((fmask & DTK_TIME_M) != DTK_TIME_M)
 	{
 		 
 		if (len == 6)
 		{
 			*tmask = DTK_TIME_M;
 			tm->tm_sec = atoi(str + 4);
 			*(str + 4) = '\0';
 			tm->tm_min = atoi(str + 2);
 			*(str + 2) = '\0';
 			tm->tm_hour = atoi(str + 0);
 
 			return DTK_TIME;
 		}
 		 
 		else if (len == 4)
 		{
 			*tmask = DTK_TIME_M;
 			tm->tm_sec = 0;
 			tm->tm_min = atoi(str + 2);
 			*(str + 2) = '\0';
 			tm->tm_hour = atoi(str + 0);
 
 			return DTK_TIME;
 		}
 	}
 
 	return -1;
 }	 