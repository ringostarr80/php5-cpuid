/*
 * File:   LibCpuId.h
 * Author: Ringo Leese
 *
 * Created on 3. Juni 2013, 16:18
 */

#ifndef LIBCPUID_H
#define	LIBCPUID_H

#ifdef ZTS
#include <php5/TSRM/TSRM.h>
#endif

struct CpuRegisters {
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
};

struct CpuRegisters cpuid(uint32_t in);

#define PUSH_PARAM(arg) zend_vm_stack_push(arg TSRMLS_CC)
#define POP_PARAM() (void)zend_vm_stack_pop(TSRMLS_CC)
#define PUSH_EO_PARAM()
#define POP_EO_PARAM()

#define CALL_METHOD_BASE(classname, name) zim_##classname##_##name

#define CALL_METHOD_HELPER(classname, name, retval, thisptr, num, param) \
  PUSH_PARAM(param); PUSH_PARAM((void*)num); \
  PUSH_EO_PARAM(); \
  CALL_METHOD_BASE(classname, name)(num, retval, NULL, thisptr, 0 TSRMLS_CC); \
  POP_EO_PARAM(); \
  POP_PARAM; POP_PARAM();

#define CALL_METHOD(classname, name, retval, thisptr) \
  CALL_METHOD_BASE(classname, name)(0, retval, NULL, thisptr, 0 TSRMLS_CC);

void init_libcpuid(TSRMLS_D);

ZEND_METHOD(LibCpuId, __construct);
ZEND_METHOD(LibCpuId, __get);
ZEND_METHOD(LibCpuId, fillRawData);
ZEND_METHOD(LibCpuId, detectVendorId);
ZEND_METHOD(LibCpuId, detectProcessorBrandString);
ZEND_METHOD(LibCpuId, detectProcessorInfoAndFeatureBits);
ZEND_METHOD(LibCpuId, detectExtendedProcessorInfoAndFeatureBits);

#endif	/* LIBCPUID_H */
