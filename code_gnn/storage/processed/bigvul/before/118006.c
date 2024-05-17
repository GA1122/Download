static inline String resourceString(const v8::Handle<v8::Function> function)
{
    String resourceName;
    int lineNumber;
    resourceInfo(function, resourceName, lineNumber);

    StringBuilder builder;
    builder.append(resourceName);
    builder.append(':');
    builder.append(String::number(lineNumber));
    return builder.toString();
}
