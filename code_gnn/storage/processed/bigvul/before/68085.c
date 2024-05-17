static char *createAccessFlagStr(ut32 flags, AccessFor forWhat) {
	#define NUM_FLAGS 18
	static const char* kAccessStrings[kAccessForMAX][NUM_FLAGS] = {
		{
			 
			"PUBLIC",            
			"PRIVATE",           
			"PROTECTED",         
			"STATIC",            
			"FINAL",             
			"?",                 
			"?",                 
			"?",                 
			"?",                 
			"INTERFACE",         
			"ABSTRACT",          
			"?",                 
			"SYNTHETIC",         
			"ANNOTATION",        
			"ENUM",              
			"?",                 
			"VERIFIED",          
			"OPTIMIZED",         
		},
		{
			 
			"PUBLIC",            
			"PRIVATE",           
			"PROTECTED",         
			"STATIC",            
			"FINAL",             
			"SYNCHRONIZED",      
			"BRIDGE",            
			"VARARGS",           
			"NATIVE",            
			"?",                 
			"ABSTRACT",          
			"STRICT",            
			"SYNTHETIC",         
			"?",                 
			"?",                 
			"MIRANDA",           
			"CONSTRUCTOR",       
			"DECLARED_SYNCHRONIZED",  
		},
		{
			 
			"PUBLIC",            
			"PRIVATE",           
			"PROTECTED",         
			"STATIC",            
			"FINAL",             
			"?",                 
			"VOLATILE",          
			"TRANSIENT",         
			"?",                 
			"?",                 
			"?",                 
			"?",                 
			"SYNTHETIC",         
			"?",                 
			"ENUM",              
			"?",                 
			"?",                 
			"?",                 
		},
	};
	const int kLongest = 21;
	int i, count;
	char* str;
	char* cp;
	count = countOnes(flags);
	cp = str = (char*) malloc (count * (kLongest + 1) + 1);
	for (i = 0; i < NUM_FLAGS; i++) {
		if (flags & 0x01) {
			const char* accessStr = kAccessStrings[forWhat][i];
			int len = strlen(accessStr);
			if (cp != str) {
				*cp++ = ' ';
			}
			memcpy(cp, accessStr, len);
			cp += len;
		}
		flags >>= 1;
	}
	*cp = '\0';
	return str;
}
