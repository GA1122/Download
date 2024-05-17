stringprep_4i (uint32_t * ucs4, size_t * len, size_t maxucs4len,
	       Stringprep_profile_flags flags,
	       const Stringprep_profile * profile)
{
  size_t i, j;
  ssize_t k;
  size_t ucs4len = *len;
  int rc;

  for (i = 0; profile[i].operation; i++)
    {
      switch (profile[i].operation)
	{
	case STRINGPREP_NFKC:
	  {
	    uint32_t *q = 0;

	    if (UNAPPLICAPLEFLAGS (flags, profile[i].flags))
	      break;

	    if (flags & STRINGPREP_NO_NFKC && !profile[i].flags)
	       
	      return STRINGPREP_FLAG_ERROR;

	    q = stringprep_ucs4_nfkc_normalize (ucs4, ucs4len);
	    if (!q)
	      return STRINGPREP_NFKC_FAILED;

	    for (ucs4len = 0; q[ucs4len]; ucs4len++)
	      ;

	    if (ucs4len >= maxucs4len)
	      {
		free (q);
		return STRINGPREP_TOO_SMALL_BUFFER;
	      }

	    memcpy (ucs4, q, ucs4len * sizeof (ucs4[0]));

	    free (q);
	  }
	  break;

	case STRINGPREP_PROHIBIT_TABLE:
	  k = stringprep_find_string_in_table (ucs4, ucs4len,
					       NULL, profile[i].table);
	  if (k != -1)
	    return STRINGPREP_CONTAINS_PROHIBITED;
	  break;

	case STRINGPREP_UNASSIGNED_TABLE:
	  if (UNAPPLICAPLEFLAGS (flags, profile[i].flags))
	    break;
	  if (flags & STRINGPREP_NO_UNASSIGNED)
	    {
	      k = stringprep_find_string_in_table
		(ucs4, ucs4len, NULL, profile[i].table);
	      if (k != -1)
		return STRINGPREP_CONTAINS_UNASSIGNED;
	    }
	  break;

	case STRINGPREP_MAP_TABLE:
	  if (UNAPPLICAPLEFLAGS (flags, profile[i].flags))
	    break;
	  rc = stringprep_apply_table_to_string
	    (ucs4, &ucs4len, maxucs4len, profile[i].table);
	  if (rc != STRINGPREP_OK)
	    return rc;
	  break;

	case STRINGPREP_BIDI_PROHIBIT_TABLE:
	case STRINGPREP_BIDI_RAL_TABLE:
	case STRINGPREP_BIDI_L_TABLE:
	  break;

	case STRINGPREP_BIDI:
	  {
	    int done_prohibited = 0;
	    int done_ral = 0;
	    int done_l = 0;
	    size_t contains_ral = SIZE_MAX;
	    size_t contains_l = SIZE_MAX;

	    for (j = 0; profile[j].operation; j++)
	      if (profile[j].operation == STRINGPREP_BIDI_PROHIBIT_TABLE)
		{
		  done_prohibited = 1;
		  k = stringprep_find_string_in_table (ucs4, ucs4len,
						       NULL,
						       profile[j].table);
		  if (k != -1)
		    return STRINGPREP_BIDI_CONTAINS_PROHIBITED;
		}
	      else if (profile[j].operation == STRINGPREP_BIDI_RAL_TABLE)
		{
		  done_ral = 1;
		  if (stringprep_find_string_in_table
		      (ucs4, ucs4len, NULL, profile[j].table) != -1)
		    contains_ral = j;
		}
	      else if (profile[j].operation == STRINGPREP_BIDI_L_TABLE)
		{
		  done_l = 1;
		  if (stringprep_find_string_in_table
		      (ucs4, ucs4len, NULL, profile[j].table) != -1)
		    contains_l = j;
		}

	    if (!done_prohibited || !done_ral || !done_l)
	      return STRINGPREP_PROFILE_ERROR;

	    if (contains_ral != SIZE_MAX && contains_l != SIZE_MAX)
	      return STRINGPREP_BIDI_BOTH_L_AND_RAL;

	    if (contains_ral != SIZE_MAX)
	      {
		if (!(stringprep_find_character_in_table
		      (ucs4[0], profile[contains_ral].table) != -1 &&
		      stringprep_find_character_in_table
		      (ucs4[ucs4len - 1], profile[contains_ral].table) != -1))
		  return STRINGPREP_BIDI_LEADTRAIL_NOT_RAL;
	      }
	  }
	  break;

	default:
	  return STRINGPREP_PROFILE_ERROR;
	  break;
	}
    }

  *len = ucs4len;

  return STRINGPREP_OK;
}
