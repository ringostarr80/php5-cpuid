/*
 * File:   php_cpuid.h
 * Author: Ringo Leese
 *
 * Created on 3. Juni 2013, 14:25
 */

#ifndef PHP_CPUID_H
#define PHP_CPUID_H 1

#define PHP_CPUID_VERSION "0.2"
#define PHP_CPUID_EXTNAME "cpuid"

#define PHP_CPUID_RES_NAME "CpuId"

#ifdef ZTS
#include <php5/TSRM/TSRM.h>
#endif

ZEND_MINIT_FUNCTION(cpuid);
ZEND_MSHUTDOWN_FUNCTION(cpuid);
ZEND_RINIT_FUNCTION(cpuid);
ZEND_MINFO_FUNCTION(cpuid);

ZEND_FUNCTION(cpuid);

extern zend_module_entry cpuid_module_entry;
#define phpext_proj4_ptr &cpuid_module_entry

#endif
