  int GetSelectedIndex() {
    base::string16 script(base::ASCIIToUTF16(kSelectID));
    script.append(base::ASCIIToUTF16(".selectedIndex"));
    int selected_index = -1;
    ExecuteJavaScriptAndReturnIntValue(script, &selected_index);
    return selected_index;
  }
