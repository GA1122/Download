void GranularityStrategyTest::SetupTextSpan(String str1,
                                            String str2,
                                            String str3,
                                            size_t sel_begin,
                                            size_t sel_end) {
  Text* text1 = GetDocument().createTextNode(str1);
  Text* text2 = GetDocument().createTextNode(str2);
  Text* text3 = GetDocument().createTextNode(str3);
  Element* span = HTMLSpanElement::Create(GetDocument());
  Element* div = GetDocument().getElementById("mytext");
  div->AppendChild(text1);
  div->AppendChild(span);
  span->AppendChild(text2);
  div->AppendChild(text3);

  GetDocument().View()->UpdateAllLifecyclePhases();

  Vector<IntPoint> letter_pos;
  Vector<IntPoint> word_middle_pos;

  TextNodeVector text_nodes;
  text_nodes.push_back(text1);
  text_nodes.push_back(text2);
  text_nodes.push_back(text3);
  ParseText(text_nodes);

  Position p1;
  Position p2;
  if (sel_begin < str1.length())
    p1 = Position(text1, sel_begin);
  else if (sel_begin < str1.length() + str2.length())
    p1 = Position(text2, sel_begin - str1.length());
  else
    p1 = Position(text3, sel_begin - str1.length() - str2.length());
  if (sel_end < str1.length())
    p2 = Position(text1, sel_end);
  else if (sel_end < str1.length() + str2.length())
    p2 = Position(text2, sel_end - str1.length());
  else
    p2 = Position(text3, sel_end - str1.length() - str2.length());

  Selection().SetSelection(
      SelectionInDOMTree::Builder().SetBaseAndExtent(p1, p2).Build());
}
