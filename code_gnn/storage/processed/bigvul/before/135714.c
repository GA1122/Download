Text* GranularityStrategyTest::SetupTranslateZ(String str) {
  SetInnerHTML(
      "<html>"
      "<head>"
      "<style>"
      "div {"
      "transform: translateZ(0);"
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
