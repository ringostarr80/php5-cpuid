#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php5/main/php.h>
#include <php5/main/php_main.h>
#include <php5/main/php_ini.h>
#include <php5/ext/standard/info.h>
#include <stdint.h>
#include <bits/wordsize.h>

#include "include/php_cpuid.h"

int le_cpuid;

static zend_function_entry cpuid_functions[] = {
    ZEND_FE(cpuid, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry cpuid_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_CPUID_EXTNAME,
    cpuid_functions,
    ZEND_MINIT(cpuid),
    ZEND_MSHUTDOWN(cpuid),
    ZEND_RINIT(cpuid),
    NULL,
	ZEND_MINFO(cpuid),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_CPUID_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

static void php_cpuid_dtor(zend_rsrc_list_entry *resource TSRMLS_DC)
{

}

ZEND_RINIT_FUNCTION(cpuid)
{
	return SUCCESS;
}

ZEND_MINIT_FUNCTION(cpuid)
{
	le_cpuid = zend_register_list_destructors_ex(php_cpuid_dtor, NULL, PHP_CPUID_RES_NAME, module_number);
	init_libcpuid(TSRMLS_C);

	return SUCCESS;
}

ZEND_MINFO_FUNCTION(cpuid)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "cpuid module", "enabled");
	php_info_print_table_row(2, "cpuid version", PHP_CPUID_VERSION);
	php_info_print_table_end();
}

ZEND_MSHUTDOWN_FUNCTION(cpuid)
{
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}

#ifdef COMPILE_DL_CPUID
ZEND_GET_MODULE(cpuid)
#endif

ZEND_FUNCTION(cpuid)
{
	long eax_input;
	uint32_t regs[4] = {0, 0, 0, 0};

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &eax_input) == FAILURE) {
		RETURN_FALSE;
	}

#if __WORDSIZE == 64
	__asm("mov %4, %%rax;\n"
		"cpuid;\n"
		"mov %%eax, %0;\n"
		"mov %%ebx, %1;\n"
		"mov %%ecx, %2;\n"
		"mov %%edx, %3;"
		:"=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
		:"r"(eax_input)
	);
#else
	__asm("mov %4, %%eax;\n"
		"cpuid;\n"
		"mov %%eax, %0;\n"
		"mov %%ebx, %1;\n"
		"mov %%ecx, %2;\n"
		"mov %%edx, %3;"
		:"=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
		:"r"(eax_input)
	);
#endif

	array_init(return_value);
	add_assoc_double(return_value, "eax", regs[0]);
	add_assoc_double(return_value, "ebx", regs[1]);
	add_assoc_double(return_value, "ecx", regs[2]);
	add_assoc_double(return_value, "edx", regs[3]);
}
