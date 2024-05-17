HTMLPreloadScanner::HTMLPreloadScanner(const HTMLParserOptions& options, const KURL& documentURL, PassOwnPtr<CachedDocumentParameters> documentParameters, const MediaValuesCached::MediaValuesCachedData& mediaValuesCachedData)
    : m_scanner(documentURL, std::move(documentParameters), mediaValuesCachedData)
    , m_tokenizer(HTMLTokenizer::create(options))
{
}
