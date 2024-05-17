 void SerializerMarkupAccumulator::appendElement(StringBuilder& result, Element* element, Namespaces* namespaces)
// void SerializerMarkupAccumulator::appendElement(StringBuilder& out, Element* element, Namespaces* namespaces)
  {
      if (!shouldIgnoreElement(element))
        MarkupAccumulator::appendElement(result, element, namespaces);
//         MarkupAccumulator::appendElement(out, element, namespaces);
  
        result.appendLiteral("<meta charset=\"");
        result.append(m_document->charset());
        if (m_document->isXHTMLDocument())
            result.appendLiteral("\" />");
        else
            result.appendLiteral("\">");
//         out.append("<meta charset=\"");
//         out.append(m_document->charset());
//         out.append("\">");
      }
  
  }