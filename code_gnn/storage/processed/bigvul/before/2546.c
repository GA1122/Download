makeJsonLexContext(text *json, bool need_escapes)
{
	return makeJsonLexContextCstringLen(VARDATA(json),
										VARSIZE(json) - VARHDRSZ,
										need_escapes);
}
