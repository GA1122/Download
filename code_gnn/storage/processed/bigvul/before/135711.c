Text* GranularityStrategyTest::SetupRotate(String str) {
  SetInnerHTML(
      "<html>"
      "<head>"
      "<style>"
      "div {"
      "transform: translate(0px,600px) rotate(90deg);"
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
