  void ExpectJohnSmithLabelsAndIdAttributes(const char* html) {
    std::vector<base::string16> id_attributes, name_attributes, labels, names,
        values;

    id_attributes.push_back(ASCIIToUTF16("firstname"));
    name_attributes.push_back(base::string16());
    labels.push_back(ASCIIToUTF16("First name:"));
    names.push_back(id_attributes.back());
    values.push_back(ASCIIToUTF16("John"));

    id_attributes.push_back(ASCIIToUTF16("lastname"));
    name_attributes.push_back(base::string16());
    labels.push_back(ASCIIToUTF16("Last name:"));
    names.push_back(id_attributes.back());
    values.push_back(ASCIIToUTF16("Smith"));

    id_attributes.push_back(ASCIIToUTF16("email"));
    name_attributes.push_back(base::string16());
    labels.push_back(ASCIIToUTF16("Email:"));
    names.push_back(id_attributes.back());
    values.push_back(ASCIIToUTF16("john@example.com"));

    ExpectLabels(html, id_attributes, name_attributes, labels, names, values);
  }
