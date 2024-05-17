PGTYPESdate_fmt_asc(date dDate, const char *fmtstring, char *outbuf)
{
	static struct
	{
		char	   *format;
		int			component;
	}			mapping[] =
	{
		 
		{
			"ddd", PGTYPES_FMTDATE_DOW_LITERAL_SHORT
		},
		{
			"dd", PGTYPES_FMTDATE_DAY_DIGITS_LZ
		},
		{
			"mmm", PGTYPES_FMTDATE_MONTH_LITERAL_SHORT
		},
		{
			"mm", PGTYPES_FMTDATE_MONTH_DIGITS_LZ
		},
		{
			"yyyy", PGTYPES_FMTDATE_YEAR_DIGITS_LONG
		},
		{
			"yy", PGTYPES_FMTDATE_YEAR_DIGITS_SHORT
		},
		{
			NULL, 0
		}
	};

	union un_fmt_comb replace_val;
	int			replace_type;

	int			i;
	int			dow;
	char	   *start_pattern;
	struct tm	tm;

	 
	strcpy(outbuf, fmtstring);

	 
	j2date(dDate + date2j(2000, 1, 1), &(tm.tm_year), &(tm.tm_mon), &(tm.tm_mday));
	dow = PGTYPESdate_dayofweek(dDate);

	for (i = 0; mapping[i].format != NULL; i++)
	{
		while ((start_pattern = strstr(outbuf, mapping[i].format)) != NULL)
		{
			switch (mapping[i].component)
			{
				case PGTYPES_FMTDATE_DOW_LITERAL_SHORT:
					replace_val.str_val = pgtypes_date_weekdays_short[dow];
					replace_type = PGTYPES_TYPE_STRING_CONSTANT;
					break;
				case PGTYPES_FMTDATE_DAY_DIGITS_LZ:
					replace_val.uint_val = tm.tm_mday;
					replace_type = PGTYPES_TYPE_UINT_2_LZ;
					break;
				case PGTYPES_FMTDATE_MONTH_LITERAL_SHORT:
					replace_val.str_val = months[tm.tm_mon - 1];
					replace_type = PGTYPES_TYPE_STRING_CONSTANT;
					break;
				case PGTYPES_FMTDATE_MONTH_DIGITS_LZ:
					replace_val.uint_val = tm.tm_mon;
					replace_type = PGTYPES_TYPE_UINT_2_LZ;
					break;
				case PGTYPES_FMTDATE_YEAR_DIGITS_LONG:
					replace_val.uint_val = tm.tm_year;
					replace_type = PGTYPES_TYPE_UINT_4_LZ;
					break;
				case PGTYPES_FMTDATE_YEAR_DIGITS_SHORT:
					replace_val.uint_val = tm.tm_year % 100;
					replace_type = PGTYPES_TYPE_UINT_2_LZ;
					break;
				default:

					 
					replace_val.str_val = " ";
					replace_type = PGTYPES_TYPE_STRING_CONSTANT;
			}
			switch (replace_type)
			{
				case PGTYPES_TYPE_STRING_MALLOCED:
				case PGTYPES_TYPE_STRING_CONSTANT:
					strncpy(start_pattern, replace_val.str_val,
							strlen(replace_val.str_val));
					if (replace_type == PGTYPES_TYPE_STRING_MALLOCED)
						free(replace_val.str_val);
					break;
				case PGTYPES_TYPE_UINT:
					{
						char	   *t = pgtypes_alloc(PGTYPES_DATE_NUM_MAX_DIGITS);

						if (!t)
							return -1;
						snprintf(t, PGTYPES_DATE_NUM_MAX_DIGITS,
								 "%u", replace_val.uint_val);
						strncpy(start_pattern, t, strlen(t));
						free(t);
					}
					break;
				case PGTYPES_TYPE_UINT_2_LZ:
					{
						char	   *t = pgtypes_alloc(PGTYPES_DATE_NUM_MAX_DIGITS);

						if (!t)
							return -1;
						snprintf(t, PGTYPES_DATE_NUM_MAX_DIGITS,
								 "%02u", replace_val.uint_val);
						strncpy(start_pattern, t, strlen(t));
						free(t);
					}
					break;
				case PGTYPES_TYPE_UINT_4_LZ:
					{
						char	   *t = pgtypes_alloc(PGTYPES_DATE_NUM_MAX_DIGITS);

						if (!t)
							return -1;
						snprintf(t, PGTYPES_DATE_NUM_MAX_DIGITS,
								 "%04u", replace_val.uint_val);
						strncpy(start_pattern, t, strlen(t));
						free(t);
					}
					break;
				default:

					 
					break;
			}
		}
	}
	return 0;
}