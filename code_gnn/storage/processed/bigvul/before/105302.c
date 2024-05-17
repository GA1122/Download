 PassRefPtr<Document> XSLTProcessor::transformToDocument(Node* sourceNode)
 {
     String resultMIMEType;
    String resultString;
    String resultEncoding;
    if (!transformToString(sourceNode, resultMIMEType, resultString, resultEncoding))
        return 0;
    return createDocumentFromSource(resultString, resultEncoding, resultMIMEType, sourceNode, 0);
}
