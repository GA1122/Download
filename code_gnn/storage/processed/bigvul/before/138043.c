AXNodeObject::findAllRadioButtonsWithSameName(HTMLInputElement* radioButton) {
  HeapVector<Member<HTMLInputElement>> allRadioButtons;
  if (!radioButton || radioButton->type() != InputTypeNames::radio)
    return allRadioButtons;

  constexpr bool kTraverseForward = true;
  constexpr bool kTraverseBackward = false;
  HTMLInputElement* firstRadioButton = radioButton;
  do {
    radioButton = RadioInputType::nextRadioButtonInGroup(firstRadioButton,
                                                         kTraverseBackward);
    if (radioButton)
      firstRadioButton = radioButton;
  } while (radioButton);

  HTMLInputElement* nextRadioButton = firstRadioButton;
  do {
    allRadioButtons.push_back(nextRadioButton);
    nextRadioButton = RadioInputType::nextRadioButtonInGroup(nextRadioButton,
                                                             kTraverseForward);
  } while (nextRadioButton);
  return allRadioButtons;
}
