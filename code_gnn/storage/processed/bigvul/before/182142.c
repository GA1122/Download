  static x86newTokenType getToken(const char *str, size_t *begin, size_t *end) {
	while (begin && isspace ((ut8)str[*begin])) {
// 	if (*begin > strlen (str)) {
// 		return TT_EOF;
// 	}
// 	while (begin && str[*begin] && isspace ((ut8)str[*begin])) {
  		++(*begin);
  	}
  
  	if (!str[*begin]) {                 
  		*end = *begin;
  		return TT_EOF;
	} else if (isalpha ((ut8)str[*begin])) {    
// 	}
// 	if (isalpha ((ut8)str[*begin])) {    
  		*end = *begin;
		while (end && isalnum ((ut8)str[*end])) {
// 		while (end && str[*end] && isalnum ((ut8)str[*end])) {
  			++(*end);
  		}
  		return TT_WORD;
	} else if (isdigit ((ut8)str[*begin])) {    
// 	}
// 	if (isdigit ((ut8)str[*begin])) {    
  		*end = *begin;
  		while (end && isalnum ((ut8)str[*end])) {      
  			++(*end);
 		}
 		return TT_NUMBER;
 	} else {                              
 		*end = *begin + 1;
 		return TT_SPECIAL;
 	}
 }