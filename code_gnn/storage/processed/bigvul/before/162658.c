bool IsQueryMatch(
    const std::string& url_query,
    const std::string& key,
    URLQueryElementMatcherCondition::QueryElementType query_element_type,
    const std::string& value,
    URLQueryElementMatcherCondition::QueryValueMatchType query_value_match_type,
    URLQueryElementMatcherCondition::Type match_type) {
  URLMatcherConditionFactory factory;

  URLMatcherCondition m1 = factory.CreateHostSuffixCondition("example.com");
  URLMatcherCondition m2 = factory.CreatePathContainsCondition("foo");
  URLMatcherConditionSet::Conditions conditions;
  conditions.insert(m1);
  conditions.insert(m2);

  URLQueryElementMatcherCondition q1(key,
                                     value,
                                     query_value_match_type,
                                     query_element_type,
                                     match_type,
                                     &factory);
  URLMatcherConditionSet::QueryConditions query_conditions;
  query_conditions.insert(q1);

  std::unique_ptr<URLMatcherSchemeFilter> scheme_filter;
  std::unique_ptr<URLMatcherPortFilter> port_filter;

  scoped_refptr<URLMatcherConditionSet> condition_set(
      new URLMatcherConditionSet(1, conditions, query_conditions,
                                 std::move(scheme_filter),
                                 std::move(port_filter)));

  GURL url("http://www.example.com/foo?" + url_query);

  URLMatcher matcher;
  URLMatcherConditionSet::Vector vector;
  vector.push_back(condition_set);
  matcher.AddConditionSets(vector);

  return matcher.MatchURL(url).size() == 1;
}
