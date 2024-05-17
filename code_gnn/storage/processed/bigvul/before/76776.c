resolveEmphasisWords(EmphasisInfo *buffer, const EmphasisClass class,
		const InString *input, unsigned int *wordBuffer) {
	int in_word = 0, in_emp = 0, word_stop;   
	int word_start = -1;					  
	unsigned int word_whole = 0;			  
	int i;

	for (i = 0; i < input->length; i++) {
		 
		wordBuffer[i] &= ~WORD_WHOLE;

		 
		if (!in_emp)
			if (buffer[i].begin & class) {
				in_emp = 1;
				buffer[i].begin &= ~class;

				 
				if (in_word) {
					word_start = i;
					word_whole = 0;
				}

				 
				if (!(wordBuffer[i] & WORD_CHAR)) word_start = -1;
			}

		 
		if (in_emp)
			if (buffer[i].end & class) {
				in_emp = 0;
				buffer[i].end &= ~class;

				if (in_word && word_start >= 0) {
					 
					word_stop = 1;
					if (wordBuffer[i] & WORD_CHAR)
						word_whole = 0;
					else
						word_stop = 0;

					 
					if (word_start + 1 == i)
						buffer[word_start].symbol |= class;
					else {
						buffer[word_start].word |= class;
						if (word_stop) {
							buffer[i].end |= class;
							buffer[i].word |= class;
						}
					}
					wordBuffer[word_start] |= word_whole;
				}
			}

		 
		if (!in_word)
			if (wordBuffer[i] & WORD_CHAR) {
				in_word = 1;
				if (in_emp) {
					word_whole = WORD_WHOLE;
					word_start = i;
				}
			}

		 
		if (in_word)
			if (!(wordBuffer[i] & WORD_CHAR)) {
				 
				if (in_emp && word_start >= 0) {
					 
					if (word_start + 1 == i)
						buffer[word_start].symbol |= class;
					else
						buffer[word_start].word |= class;
					wordBuffer[word_start] |= word_whole;
				}

				in_word = 0;
				word_whole = 0;
				word_start = -1;
			}
	}

	 
	if (in_emp) {
		buffer[i].end &= ~class;

		if (in_word)
			if (word_start >= 0) {
				 
				if (word_start + 1 == i)
					buffer[word_start].symbol |= class;
				else
					buffer[word_start].word |= class;
				wordBuffer[word_start] |= word_whole;
			}
	}
}
