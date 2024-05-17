xmlParse3986DecOctet(const char **str) {
 const char *cur = *str;

 if (!(ISA_DIGIT(cur)))
 return(1);
 if (!ISA_DIGIT(cur+1))
	cur++;
 else if ((*cur != '0') && (ISA_DIGIT(cur + 1)) && (!ISA_DIGIT(cur+2)))
	cur += 2;
 else if ((*cur == '1') && (ISA_DIGIT(cur + 1)) && (ISA_DIGIT(cur + 2)))
	cur += 3;
 else if ((*cur == '2') && (*(cur + 1) >= '0') &&
 (*(cur + 1) <= '4') && (ISA_DIGIT(cur + 2)))
	cur += 3;
 else if ((*cur == '2') && (*(cur + 1) == '5') &&
 (*(cur + 2) >= '0') && (*(cur + 1) <= '5'))
	cur += 3;
 else
 return(1);
 *str = cur;
 return(0);
}
