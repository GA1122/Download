raptor_rdfxml_parse_recognise_syntax(raptor_parser_factory* factory, 
                                     const unsigned char *buffer, size_t len,
                                     const unsigned char *identifier, 
                                     const unsigned char *suffix, 
                                     const char *mime_type)
{
  int score = 0;
  
  if(suffix) {
    if(!strcmp((const char*)suffix, "rdf") || 
       !strcmp((const char*)suffix, "rdfs") ||
       !strcmp((const char*)suffix, "foaf") ||
       !strcmp((const char*)suffix, "doap") ||
       !strcmp((const char*)suffix, "owl") ||
       !strcmp((const char*)suffix, "daml"))
      score = 9;
    if(!strcmp((const char*)suffix, "rss"))
      score = 3;
  }
  
  if(identifier) {
    if(strstr((const char*)identifier, "rss1"))
      score += 5;
    else if(!suffix && strstr((const char*)identifier, "rss"))
      score += 3;
    else if(!suffix && strstr((const char*)identifier, "rdf"))
      score += 2;
    else if(!suffix && strstr((const char*)identifier, "RDF"))
      score += 2;
  }
  
  if(mime_type) {
    if(strstr((const char*)mime_type, "html"))
      score -= 4;
    else if(!strcmp((const char*)mime_type, "text/rdf"))
      score += 7;
    else if(!strcmp((const char*)mime_type, "application/xml"))
      score += 5;
  }

  if(buffer && len) {
     
#define  HAS_RDF_XMLNS1 (raptor_memstr((const char*)buffer, len, "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#") != NULL)
#define  HAS_RDF_XMLNS2 (raptor_memstr((const char*)buffer, len, "xmlns:rdf='http://www.w3.org/1999/02/22-rdf-syntax-ns#") != NULL)
#define  HAS_RDF_XMLNS3 (raptor_memstr((const char*)buffer, len, "xmlns=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#") != NULL)
#define  HAS_RDF_XMLNS4 (raptor_memstr((const char*)buffer, len, "xmlns='http://www.w3.org/1999/02/22-rdf-syntax-ns#") != NULL)
#define  HAS_RDF_ENTITY1 (raptor_memstr((const char*)buffer, len, "<!ENTITY rdf 'http://www.w3.org/1999/02/22-rdf-syntax-ns#'>") != NULL)
#define  HAS_RDF_ENTITY2 (raptor_memstr((const char*)buffer, len, "<!ENTITY rdf \"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">") != NULL)
#define  HAS_RDF_ENTITY3 (raptor_memstr((const char*)buffer, len, "xmlns:rdf=\"&rdf;\"") != NULL)
#define  HAS_RDF_ENTITY4 (raptor_memstr((const char*)buffer, len, "xmlns:rdf='&rdf;'") != NULL)
#define  HAS_HTML_NS (raptor_memstr((const char*)buffer, len, "http://www.w3.org/1999/xhtml") != NULL)
#define  HAS_HTML_ROOT (raptor_memstr((const char*)buffer, len, "<html") != NULL)

    if(!HAS_HTML_NS && !HAS_HTML_ROOT &&
       (HAS_RDF_XMLNS1 || HAS_RDF_XMLNS2 || HAS_RDF_XMLNS3 || HAS_RDF_XMLNS4 ||
        HAS_RDF_ENTITY1 || HAS_RDF_ENTITY2 || HAS_RDF_ENTITY3 || HAS_RDF_ENTITY4)
      ) {
      int has_rdf_RDF = (raptor_memstr((const char*)buffer, len, "<rdf:RDF") != NULL);
      int has_rdf_Description = (raptor_memstr((const char*)buffer, len, "rdf:Description") != NULL);
      int has_rdf_about = (raptor_memstr((const char*)buffer, len, "rdf:about") != NULL);

      score += 7;
      if(has_rdf_RDF)
        score++;
      if(has_rdf_Description)
        score++;
      if(has_rdf_about)
        score++;
    }
  }
  
  return score;
}