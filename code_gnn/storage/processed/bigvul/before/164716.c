  void ExpectJohnSmithLabelsAndNameAttributes(const char* html) {
    std::vector<base::string16> id_attributes, name_attributes, labels, names,
        values;
    id_attributes.push_back(base::string16());
    name_attributes.push_back(ASCIIToUTF16("firstname"));
    labels.push_back(ASCIIToUTF16("First name:"));
    names.push_back(name_attributes.back());
    values.push_back(ASCIIToUTF16("John"));

    id_attributes.push_back(base::string16());
    name_attributes.push_back(ASCIIToUTF16("lastname"));
    labels.push_back(ASCIIToUTF16("Last name:"));
    names.push_back(name_attributes.back());
    values.push_back(ASCIIToUTF16("Smith"));

    id_attributes.push_back(base::string16());
    name_attributes.push_back(ASCIIToUTF16("email"));
    labels.push_back(ASCIIToUTF16("Email:"));
    names.push_back(name_attributes.back());
    values.push_back(ASCIIToUTF16("john@example.com"));
    ExpectLabels(html, id_attributes, name_attributes, labels, names, values);
  }
