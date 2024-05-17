test_bson_validate_dbref (void)
{
   size_t offset;
   bson_t dbref, child, child2;

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_UTF8 (&child, "$ref", "foo");
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (!bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_UTF8 (&child, "$ref", "foo");
      BSON_APPEND_UTF8 (&child, "extra", "field");
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (!bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_UTF8 (&dbref, "$ref", "foo");
      BSON_APPEND_UTF8 (&dbref, "$id", "bar");

      BSON_ASSERT (!bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_UTF8 (&child, "extra", "field");
      BSON_APPEND_UTF8 (&child, "$ref", "foo");
      BSON_APPEND_UTF8 (&child, "$id", "bar");
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (!bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_UTF8 (&child, "$ref", "foo");
      BSON_APPEND_UTF8 (&child, "$db", "bar");
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (!bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_INT32 (&child, "$ref", 1);
      BSON_APPEND_UTF8 (&child, "$id", "bar");
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (!bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_INT32 (&child, "$ref", 1);
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (!bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_UTF8 (&child, "$ref", "foo");
      BSON_APPEND_UTF8 (&child, "$id", "bar");
      BSON_APPEND_INT32 (&child, "$db", 1);
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (!bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_UTF8 (&child, "$ref", "foo");
      BSON_APPEND_UTF8 (&child, "$id", "bar");
      BSON_APPEND_INT32 (&child, "$db", 1);
      BSON_APPEND_UTF8 (&child, "extra", "field");
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (!bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_UTF8 (&child, "$ref", "foo");
      BSON_APPEND_UTF8 (&child, "$id", "bar");
      BSON_APPEND_UTF8 (&child, "extra", "field");
      BSON_APPEND_UTF8 (&child, "$db", "baz");
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (!bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_UTF8 (&child, "$ref", "foo");
      BSON_APPEND_UTF8 (&child, "$id", "bar");
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_UTF8 (&child, "$ref", "foo");
      BSON_APPEND_DOCUMENT_BEGIN (&child, "$id", &child2);
      BSON_APPEND_UTF8 (&child2, "$ref", "foo2");
      BSON_APPEND_UTF8 (&child2, "$id", "bar2");
      BSON_APPEND_UTF8 (&child2, "$db", "baz2");
      bson_append_document_end (&child, &child2);
      BSON_APPEND_UTF8 (&child, "$db", "baz");
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_UTF8 (&child, "$ref", "foo");
      BSON_APPEND_UTF8 (&child, "$id", "bar");
      BSON_APPEND_UTF8 (&child, "$db", "baz");
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }

    
   {
      bson_init (&dbref);
      BSON_APPEND_DOCUMENT_BEGIN (&dbref, "dbref", &child);
      BSON_APPEND_UTF8 (&child, "$ref", "foo");
      BSON_APPEND_UTF8 (&child, "$id", "bar");
      BSON_APPEND_UTF8 (&child, "$db", "baz");
      BSON_APPEND_UTF8 (&child, "extra", "field");
      bson_append_document_end (&dbref, &child);

      BSON_ASSERT (bson_validate (&dbref, BSON_VALIDATE_DOLLAR_KEYS, &offset));

      bson_destroy (&dbref);
   }
}
