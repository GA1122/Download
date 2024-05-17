Text* GranularityStrategyTest::SetupTransform(String str) {
  SetInnerHTML(
      "<html>"
      "<head>"
      "<style>"
      "div {"
      "transform: scale(1,-1) translate(0,-100px);"
      "}"
      "</style>"
      "</head>"
      "<body>"
      "<div id='mytext'></div>"
      "</body>"
      "</html>");

  Text* text = GetDocument().createTextNode(str);
  Element* div = GetDocument().getElementById("mytext");
  div->AppendChild(text);

  GetDocument().View()->UpdateAllLifecyclePhases();

  ParseText(text);
  return text;
}
