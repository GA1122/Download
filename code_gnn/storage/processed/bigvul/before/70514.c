void yyerror(
    yyscan_t yyscanner,
    RE_LEX_ENVIRONMENT* lex_env,
    const char *error_message)
{

  if (lex_env->last_error_code == ERROR_SUCCESS)
  {
    lex_env->last_error_code = ERROR_INVALID_REGULAR_EXPRESSION;

    strlcpy(
        lex_env->last_error_message,
        error_message,
        sizeof(lex_env->last_error_message));
  }
}
