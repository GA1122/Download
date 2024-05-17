concat_opt_exact(OptStr* to, OptStr* add, OnigEncoding enc)
{
  int i, j, len, r;
  UChar *p, *end;
  OptAnc tanc;

  if (add->case_fold != 0) {
    if (! to->case_fold) {
      if (to->len > 1 || to->len >= add->len) return 0;   

      to->case_fold = 1;
    }
    else {
      if (to->good_case_fold != 0) {
        if (add->good_case_fold == 0) return 0;
      }
    }
  }

  r = 0;
  p = add->s;
  end = p + add->len;
  for (i = to->len; p < end; ) {
    len = enclen(enc, p);
    if (i + len > OPT_EXACT_MAXLEN) {
      r = 1;  
      break;
    }
    for (j = 0; j < len && p < end; j++)
      to->s[i++] = *p++;
  }

  to->len = i;
  to->reach_end = (p == end ? add->reach_end : 0);

  concat_opt_anc_info(&tanc, &to->anc, &add->anc, 1, 1);
  if (! to->reach_end) tanc.right = 0;
  copy_opt_anc_info(&to->anc, &tanc);

  return r;
}