void ImageTokenizer::createDocumentStructure()
{
    ExceptionCode ec;
    
    RefPtr<Element> rootElement = m_doc->createElementNS(xhtmlNamespaceURI, "html", ec);
    m_doc->appendChild(rootElement, ec);
    
    RefPtr<Element> body = m_doc->createElementNS(xhtmlNamespaceURI, "body", ec);
    body->setAttribute(styleAttr, "margin: 0px;");
    
    rootElement->appendChild(body, ec);
    
    RefPtr<Element> imageElement = m_doc->createElementNS(xhtmlNamespaceURI, "img", ec);
    
    m_imageElement = static_cast<HTMLImageElement *>(imageElement.get());
    m_imageElement->setAttribute(styleAttr, "-webkit-user-select: none");        
    m_imageElement->setLoadManually(true);
    m_imageElement->setSrc(m_doc->URL());
    
    body->appendChild(imageElement, ec);    
}
