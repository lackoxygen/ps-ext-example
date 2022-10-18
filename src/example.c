/* example extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_example.h"
#include "example_arginfo.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

/* {{{ void test1() */
PHP_FUNCTION(dump)
{
        zval *zv_ptr;

if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &zv_ptr) == FAILURE) {
    return;
}

    try_again:
    switch (Z_TYPE_P(zv_ptr)) {
        case IS_NULL:
            php_printf("NULL: null\n");
        break;
        case IS_TRUE:
            php_printf("BOOL: true\n");
        break;
        case IS_FALSE:
            php_printf("BOOL: false\n");
        break;
        case IS_LONG:
            php_printf("LONG: %ld\n", Z_LVAL_P(zv_ptr));
        break;
        case IS_DOUBLE:
            php_printf("DOUBLE: %g\n", Z_DVAL_P(zv_ptr));
        break;
        case IS_STRING:
            php_printf("STRING: value=\"");
        PHPWRITE(Z_STRVAL_P(zv_ptr), Z_STRLEN_P(zv_ptr));
        php_printf("\", length=%zd\n", Z_STRLEN_P(zv_ptr));
        break;
        case IS_RESOURCE:
            php_printf("RESOURCE: id=%d\n", Z_RES_HANDLE_P(zv_ptr));
        break;
        case IS_ARRAY:
            php_printf("ARRAY: hashtable=%p\n", Z_ARRVAL_P(zv_ptr));
        break;
        case IS_OBJECT:
            php_printf("OBJECT: object=%p\n", Z_OBJ_P(zv_ptr));
        break;
        case IS_REFERENCE:
            // For references, remove the reference wrapper and try again.
            // Yes, you are allowed to use goto for this purpose!
            php_printf("REFERENCE: ");
        zv_ptr = Z_REFVAL_P(zv_ptr);
        goto try_again;
        EMPTY_SWITCH_DEFAULT_CASE() // Assert that all types are handled.
    }
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(example)
{
#if defined(ZTS) && defined(COMPILE_DL_EXAMPLE)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(example)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "example support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ example_module_entry */
zend_module_entry example_module_entry = {
	STANDARD_MODULE_HEADER,
	"example",					/* Extension name */
	ext_functions,					/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(example),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(example),			/* PHP_MINFO - Module info */
	PHP_EXAMPLE_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_EXAMPLE
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(example)
#endif
