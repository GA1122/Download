hyphenate(const widechar *word, int wordSize, char *hyphens,
		const TranslationTableHeader *table) {
	widechar *prepWord;
	int i, k, limit;
	int stateNum;
	widechar ch;
	HyphenationState *statesArray =
			(HyphenationState *)&table->ruleArea[table->hyphenStatesArray];
	HyphenationState *currentState;
	HyphenationTrans *transitionsArray;
	char *hyphenPattern;
	int patternOffset;
	if (!table->hyphenStatesArray || (wordSize + 3) > MAXSTRING) return 0;
	prepWord = (widechar *)calloc(wordSize + 3, sizeof(widechar));
	 
	prepWord[0] = '.';
	for (i = 0; i < wordSize; i++) {
		prepWord[i + 1] = (findCharOrDots(word[i], 0, table))->lowercase;
		hyphens[i] = '0';
	}
	prepWord[wordSize + 1] = '.';

	 
	stateNum = 0;

	for (i = 0; i < wordSize + 2; i++) {
		ch = prepWord[i];
		while (1) {
			if (stateNum == 0xffff) {
				stateNum = 0;
				goto nextLetter;
			}
			currentState = &statesArray[stateNum];
			if (currentState->trans.offset) {
				transitionsArray =
						(HyphenationTrans *)&table->ruleArea[currentState->trans.offset];
				for (k = 0; k < currentState->numTrans; k++) {
					if (transitionsArray[k].ch == ch) {
						stateNum = transitionsArray[k].newState;
						goto stateFound;
					}
				}
			}
			stateNum = currentState->fallbackState;
		}
	stateFound:
		currentState = &statesArray[stateNum];
		if (currentState->hyphenPattern) {
			hyphenPattern = (char *)&table->ruleArea[currentState->hyphenPattern];
			patternOffset = i + 1 - (int)strlen(hyphenPattern);

			 
			limit = MIN((int)strlen(hyphenPattern), wordSize - patternOffset);
			for (k = 0; k < limit; k++) {
				if (hyphens[patternOffset + k] < hyphenPattern[k])
					hyphens[patternOffset + k] = hyphenPattern[k];
			}
		}
	nextLetter:;
	}
	hyphens[wordSize] = 0;
	free(prepWord);
	return 1;
}
