	bool testNormalizeMaskHelper(const wchar_t * uriText, unsigned int expectedMask) {
		UriParserStateW state;
		UriUriW uri;
		state.uri = &uri;
		int res = uriParseUriW(&state, uriText);
		if (res != 0) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		const unsigned int maskBefore = uriNormalizeSyntaxMaskRequiredW(&uri);
		if (maskBefore != expectedMask) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		res = uriNormalizeSyntaxW(&uri);
		if (res != 0) {
			uriFreeUriMembersW(&uri);
			return false;
		}

		const unsigned int maskAfter = uriNormalizeSyntaxMaskRequiredW(&uri);
		uriFreeUriMembersW(&uri);

		uriFreeUriMembersW(&uri);

		return (maskAfter == URI_NORMALIZED);
	}
