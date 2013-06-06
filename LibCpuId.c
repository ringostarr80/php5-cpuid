#include <php5/main/php.h>
#include <php5/main/php_main.h>
#include <php5/main/php_ini.h>
#include <php5/ext/standard/info.h>
#include <stdint.h>

#include "include/LibCpuId.h"

/**
 * @param in input for cpu register eax
 * @return output of the 4 cpu registers eax, ebx, ecx and edx
 */
struct CpuRegisters cpuid(uint32_t in) {
	struct CpuRegisters regs = {0, 0, 0, 0};

	__asm("mov %4, %%eax;"
		"cpuid;"
		"mov %%eax, %0;"
		"mov %%ebx, %1;"
		"mov %%ecx, %2;"
		"mov %%edx, %3;"
		:"=a"(regs.eax), "=b"(regs.ebx), "=c"(regs.ecx), "=d"(regs.edx)
		:"r"(in)
	);

	return regs;
}

zend_class_entry *ce_libcpuid;

ZEND_BEGIN_ARG_INFO_EX(__get_args, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

static zend_function_entry libcpuid_methods[] = {
	ZEND_ME(LibCpuId, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	ZEND_ME(LibCpuId, __get, __get_args, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	ZEND_ME(LibCpuId, fillRawData, NULL, ZEND_ACC_PRIVATE)
	ZEND_ME(LibCpuId, detectVendorId, NULL, ZEND_ACC_PRIVATE)
	ZEND_ME(LibCpuId, detectProcessorBrandString, NULL, ZEND_ACC_PRIVATE)
	ZEND_ME(LibCpuId, detectProcessorInfoAndFeatureBits, NULL, ZEND_ACC_PRIVATE)
	ZEND_ME(LibCpuId, detectExtendedProcessorInfoAndFeatureBits, NULL, ZEND_ACC_PRIVATE)
	{NULL, NULL, NULL}
};

void init_libcpuid(TSRMLS_D) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "LibCpuId", libcpuid_methods);
	ce_libcpuid = zend_register_internal_class(&ce TSRMLS_CC);

	/* fields */
	zend_declare_property_null(ce_libcpuid, "_rawData", strlen("_rawData"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(ce_libcpuid, "_processorInfo", strlen("_processorInfo"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_null(ce_libcpuid, "_featureBits", strlen("_featureBits"), ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_long(ce_libcpuid, "_cpuidLevel", strlen("_cpuidLevel"), 0, ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_string(ce_libcpuid, "_vendorId", strlen("_vendorId"), "", ZEND_ACC_PRIVATE TSRMLS_CC);
	zend_declare_property_string(ce_libcpuid, "_processorBrandString", strlen("_processorBrandString"), "", ZEND_ACC_PRIVATE TSRMLS_CC);
}

ZEND_METHOD(LibCpuId, __construct) {
	zval *raw_data = NULL, *processor_info = NULL, *feature_bits = NULL;
	zval *object = getThis();

	ALLOC_INIT_ZVAL(raw_data);
	ALLOC_INIT_ZVAL(processor_info);
	ALLOC_INIT_ZVAL(feature_bits);
	array_init(raw_data);
	array_init(processor_info);
	array_init(feature_bits);
	zend_update_property(ce_libcpuid, object, "_rawData", strlen("_rawData"), raw_data TSRMLS_CC);
	zend_update_property(ce_libcpuid, object, "_processorInfo", strlen("_processorInfo"), processor_info TSRMLS_CC);
	zend_update_property(ce_libcpuid, object, "_featureBits", strlen("_featureBits"), feature_bits TSRMLS_CC);

	CALL_METHOD(LibCpuId, fillRawData, return_value, object);
	CALL_METHOD(LibCpuId, detectVendorId, return_value, object);
	CALL_METHOD(LibCpuId, detectProcessorBrandString, return_value, object);
	CALL_METHOD(LibCpuId, detectProcessorInfoAndFeatureBits, return_value, object);
	CALL_METHOD(LibCpuId, detectExtendedProcessorInfoAndFeatureBits, return_value, object);
}

ZEND_METHOD(LibCpuId, __get) {
	zval *raw_data, *vendor_id, *processor_brand_string, *cpuid_level, *processor_info, *feature_bits;
	zval *object = getThis();
	char *name;
	int name_length;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &name, &name_length) == FAILURE) {
		RETURN_FALSE;
	}

	if (strcmp(name, "RawData") == 0) {
		raw_data = zend_read_property(ce_libcpuid, object, "_rawData", strlen("_rawData"), 0 TSRMLS_CC);
		RETURN_ZVAL(raw_data, 1, 0);
	} else if (strcmp(name, "VendorId") == 0) {
		vendor_id = zend_read_property(ce_libcpuid, object, "_vendorId", strlen("_vendorId"), 0 TSRMLS_CC);
		RETURN_ZVAL(vendor_id, 1, 0);
	} else if (strcmp(name, "ProcessorBrandString") == 0) {
		processor_brand_string = zend_read_property(ce_libcpuid, object, "_processorBrandString", strlen("_processorBrandString"), 0 TSRMLS_CC);
		RETURN_ZVAL(processor_brand_string, 1, 0);
	} else if (strcmp(name, "CpuIdLevel") == 0) {
		cpuid_level = zend_read_property(ce_libcpuid, object, "_cpuidLevel", strlen("_cpuidLevel"), 0 TSRMLS_CC);
		RETURN_ZVAL(cpuid_level, 1, 0);
	} else if (strcmp(name, "ProcessorInfo") == 0) {
		processor_info = zend_read_property(ce_libcpuid, object, "_processorInfo", strlen("_processorInfo"), 0 TSRMLS_CC);
		RETURN_ZVAL(processor_info, 1, 0);
	} else if (strcmp(name, "FeatureBits") == 0) {
		feature_bits = zend_read_property(ce_libcpuid, object, "_featureBits", strlen("_featureBits"), 0 TSRMLS_CC);
		RETURN_ZVAL(feature_bits, 1, 0);
	}

	RETURN_NULL();
}

ZEND_METHOD(LibCpuId, fillRawData) {
	zval *raw_data, *current_data;
	zval *object = getThis();
	struct CpuRegisters regs = cpuid(0);
	uint32_t highest_input = regs.eax, i, highestExtendedInput;

	zend_update_property_long(ce_libcpuid, object, "_cpuidLevel", strlen("_cpuidLevel"), regs.eax TSRMLS_CC);

	raw_data = zend_read_property(ce_libcpuid, getThis(), "_rawData", strlen("_rawData"), 0 TSRMLS_CC);
	ALLOC_INIT_ZVAL(current_data);
	array_init(current_data);

	add_assoc_long(current_data, "eax", regs.eax);
	add_assoc_long(current_data, "ebx", regs.ebx);
	add_assoc_long(current_data, "ecx", regs.ecx);
	add_assoc_long(current_data, "edx", regs.edx);
	add_index_zval(raw_data, 0, current_data);

	for(i = 1; i <= highest_input; i++) {
		regs = cpuid(i);
		ALLOC_INIT_ZVAL(current_data);
		array_init(current_data);

		add_assoc_long(current_data, "eax", regs.eax);
		add_assoc_long(current_data, "ebx", regs.ebx);
		add_assoc_long(current_data, "ecx", regs.ecx);
		add_assoc_long(current_data, "edx", regs.edx);
		add_index_zval(raw_data, i, current_data);
	}

	// extended
	regs = cpuid(0x80000000);
	highestExtendedInput = regs.eax;
	if (highestExtendedInput > 0x80000000) {
		ALLOC_INIT_ZVAL(current_data);
		array_init(current_data);

		add_assoc_long(current_data, "eax", regs.eax);
		add_assoc_long(current_data, "ebx", regs.ebx);
		add_assoc_long(current_data, "ecx", regs.ecx);
		add_assoc_long(current_data, "edx", regs.edx);
		add_index_zval(raw_data, 0x80000000, current_data);

		for(i = 0x80000001; i <= highestExtendedInput; i++) {
			regs = cpuid(i);
			ALLOC_INIT_ZVAL(current_data);
			array_init(current_data);

			add_assoc_long(current_data, "eax", regs.eax);
			add_assoc_long(current_data, "ebx", regs.ebx);
			add_assoc_long(current_data, "ecx", regs.ecx);
			add_assoc_long(current_data, "edx", regs.edx);
			add_index_zval(raw_data, i, current_data);
		}
	}
}

ZEND_METHOD(LibCpuId, detectVendorId) {
	struct CpuRegisters regs = cpuid(0);
	char vendorID[13];
	zval *object = getThis();

	vendorID[0] = (char)((regs.ebx) & 0xFF);
	vendorID[1] = (char)((regs.ebx >> 8) & 0xFF);
	vendorID[2] = (char)((regs.ebx >> 16) & 0xFF);
	vendorID[3] = (char)((regs.ebx >> 24));
	vendorID[4] = (char)((regs.edx) & 0xFF);
	vendorID[5] = (char)((regs.edx >> 8) & 0xFF);
	vendorID[6] = (char)((regs.edx >> 16) & 0xFF);
	vendorID[7] = (char)((regs.edx >> 24) & 0xFF);
	vendorID[8] = (char)((regs.ecx) & 0xFF);
	vendorID[9] = (char)((regs.ecx >> 8) & 0xFF);
	vendorID[10] = (char)((regs.ecx >> 16) & 0xFF);
	vendorID[11] = (char)((regs.ecx >> 24) & 0xFF);
	vendorID[12] = '\0';

	zend_update_property_string(ce_libcpuid, object, "_vendorId", strlen("_vendorId"), vendorID TSRMLS_CC);
}

ZEND_METHOD(LibCpuId, detectProcessorBrandString) {
	zval *object = getThis();
	uint32_t i;
	struct CpuRegisters regs = cpuid(0x80000000);
	char processorBrandString[49];

	if (regs.eax < 0x80000004) { // feature is not supported
		return;
	}

	for(i = 0; i < 3; i++) {
		int charIndexOffset = i * 16;

		regs = cpuid(0x80000002 + i);

		processorBrandString[charIndexOffset + 0] = (char)((regs.eax) & 0xFF);
		processorBrandString[charIndexOffset + 1] = (char)((regs.eax >> 8) & 0xFF);
		processorBrandString[charIndexOffset + 2] = (char)((regs.eax >> 16) & 0xFF);
		processorBrandString[charIndexOffset + 3] = (char)((regs.eax >> 24));
		processorBrandString[charIndexOffset + 4] = (char)((regs.ebx) & 0xFF);
		processorBrandString[charIndexOffset + 5] = (char)((regs.ebx >> 8) & 0xFF);
		processorBrandString[charIndexOffset + 6] = (char)((regs.ebx >> 16) & 0xFF);
		processorBrandString[charIndexOffset + 7] = (char)((regs.ebx >> 24));
		processorBrandString[charIndexOffset + 8] = (char)((regs.ecx) & 0xFF);
		processorBrandString[charIndexOffset + 9] = (char)((regs.ecx >> 8) & 0xFF);
		processorBrandString[charIndexOffset + 10] = (char)((regs.ecx >> 16) & 0xFF);
		processorBrandString[charIndexOffset + 11] = (char)((regs.ecx >> 24) & 0xFF);
		processorBrandString[charIndexOffset + 12] = (char)((regs.edx) & 0xFF);
		processorBrandString[charIndexOffset + 13] = (char)((regs.edx >> 8) & 0xFF);
		processorBrandString[charIndexOffset + 14] = (char)((regs.edx >> 16) & 0xFF);
		processorBrandString[charIndexOffset + 15] = (char)((regs.edx >> 24) & 0xFF);
	}
	processorBrandString[48] = '\0';

	zend_update_property_string(ce_libcpuid, object, "_processorBrandString", strlen("_processorBrandString"), processorBrandString TSRMLS_CC);
}

ZEND_METHOD(LibCpuId, detectProcessorInfoAndFeatureBits) {
	struct CpuRegisters regs = cpuid(1);
	zval *processor_info, *feature_bits;

	processor_info = zend_read_property(ce_libcpuid, getThis(), "_processorInfo", strlen("_processorInfo"), 0 TSRMLS_CC);
	feature_bits = zend_read_property(ce_libcpuid, getThis(), "_featureBits", strlen("_featureBits"), 0 TSRMLS_CC);

	add_assoc_long(processor_info, "Stepping", (regs.eax) & 0x0F);
	add_assoc_long(processor_info, "Model", (regs.eax >> 4) & 0x0F);
	add_assoc_long(processor_info, "Family", (regs.eax >> 8) & 0x0F);
	add_assoc_long(processor_info, "ProcessorType", (regs.eax >> 12) & 0x03);
	add_assoc_long(processor_info, "ExtendedModel", (regs.eax >> 16) & 0x0F);
	add_assoc_long(processor_info, "ExtendedFamily", (regs.eax >> 20) & 0xFF);
	add_assoc_long(processor_info, "Raw", regs.eax);

	add_assoc_bool(feature_bits, "PrescottNewInstructions", ((regs.ecx & 0x01) > 0));
	add_assoc_bool(feature_bits, "PCLMULQDQ", ((regs.ecx & 0x02) > 0));
	add_assoc_bool(feature_bits, "DebugStore64Bit", ((regs.ecx & 0x04) > 0));
	add_assoc_bool(feature_bits, "MONITORAndMWAITInstructions", ((regs.ecx & 0x08) > 0));
	add_assoc_bool(feature_bits, "CPLQualifiedDebugStore", ((regs.ecx & 0x10) > 0));
	add_assoc_bool(feature_bits, "VirtualMachineExtensions", ((regs.ecx & 0x20) > 0));
	add_assoc_bool(feature_bits, "SaferModeExtensions", ((regs.ecx & 0x40) > 0));
	add_assoc_bool(feature_bits, "EnhancedSpeedStep", ((regs.ecx & 0x80) > 0));
	add_assoc_bool(feature_bits, "ThermalMonitor2", ((regs.ecx & 0x100) > 0));
	add_assoc_bool(feature_bits, "SupplementalSSE3Instructions", ((regs.ecx & 0x200) > 0));
	add_assoc_bool(feature_bits, "ContextID", ((regs.ecx & 0x400) > 0));

	add_assoc_bool(feature_bits, "FusedMultiplyAdd", ((regs.ecx & 0x1000) > 0));
	add_assoc_bool(feature_bits, "CMPXCHG16BInstruction", ((regs.ecx & 0x2000) > 0));
	add_assoc_bool(feature_bits, "CanDisableSendingTaskPriorityMessages", ((regs.ecx & 0x4000) > 0));
	add_assoc_bool(feature_bits, "PerfmonAndDebugCapability", ((regs.ecx & 0x8000) > 0));

	add_assoc_bool(feature_bits, "ProcessContextIdentifiers", ((regs.ecx & 0x20000) > 0));
	add_assoc_bool(feature_bits, "DirectCacheAccessForDMAWrites", ((regs.ecx & 0x40000) > 0));
	add_assoc_bool(feature_bits, "SSE4_1Instructions", ((regs.ecx & 0x80000) > 0));
	add_assoc_bool(feature_bits, "SSE4_2Instructions", ((regs.ecx & 0x100000) > 0));
	add_assoc_bool(feature_bits, "x2APICSupport", ((regs.ecx & 0x200000) > 0));
	add_assoc_bool(feature_bits, "MOVBEInstruction", ((regs.ecx & 0x400000) > 0));
	add_assoc_bool(feature_bits, "POPCNTInstruction", ((regs.ecx & 0x800000) > 0));
	add_assoc_bool(feature_bits, "TSCDeadlineSupport", ((regs.ecx & 0x1000000) > 0));
	add_assoc_bool(feature_bits, "AESInstructionSet", ((regs.ecx & 0x2000000) > 0));
	add_assoc_bool(feature_bits, "XSAVE_XRESTOR_XSETBV_XGETBV_Support", ((regs.ecx & 0x4000000) > 0));
	add_assoc_bool(feature_bits, "XSAVEEnabledByOS", ((regs.ecx & 0x8000000) > 0));
	add_assoc_bool(feature_bits, "AdvancedVectorExtensions", ((regs.ecx & 0x10000000) > 0));
	add_assoc_bool(feature_bits, "CVT16InstructionSet", ((regs.ecx & 0x20000000) > 0));
	add_assoc_bool(feature_bits, "RDRANDSupport", ((regs.ecx & 0x40000000) > 0));
	add_assoc_bool(feature_bits, "RunningOnAHypervisor", ((regs.ecx & 0x80000000) > 0));

	add_assoc_bool(feature_bits, "Onboardx87FPU", ((regs.edx & 0x01) > 0));
	add_assoc_bool(feature_bits, "VirtualModeExtensions", ((regs.edx & 0x02) > 0));
	add_assoc_bool(feature_bits, "DebuggingExtensions", ((regs.edx & 0x04) > 0));
	add_assoc_bool(feature_bits, "PageSizeExtension", ((regs.edx & 0x08) > 0));
	add_assoc_bool(feature_bits, "TimeStampCounter", ((regs.edx & 0x10) > 0));
	add_assoc_bool(feature_bits, "ModelSpecificRegisters", ((regs.edx & 0x20) > 0));
	add_assoc_bool(feature_bits, "PhysicalAddressExtension", ((regs.edx & 0x40) > 0));
	add_assoc_bool(feature_bits, "MachineCheckException", ((regs.edx & 0x80) > 0));
	add_assoc_bool(feature_bits, "CMPXCHG8Instruction", ((regs.edx & 0x100) > 0));
	add_assoc_bool(feature_bits, "OnboardAPIC", ((regs.edx & 0x200) > 0));

	add_assoc_bool(feature_bits, "SYSENTERAndSYSEXITInstructions", ((regs.edx & 0x800) > 0));
	add_assoc_bool(feature_bits, "MemoryTypeRangeRegisters", ((regs.edx & 0x1000) > 0));
	add_assoc_bool(feature_bits, "PageGlobalEnableBitInCR4", ((regs.edx & 0x2000) > 0));
	add_assoc_bool(feature_bits, "MachineCheckArchitecture", ((regs.edx & 0x4000) > 0));
	add_assoc_bool(feature_bits, "ConditionalMoveAndFCMOVInstructions", ((regs.edx & 0x8000) > 0));
	add_assoc_bool(feature_bits, "PageAttributeTable", ((regs.edx & 0x10000) > 0));
	add_assoc_bool(feature_bits, "PageSizeExtension36Bit", ((regs.edx & 0x20000) > 0));
	add_assoc_bool(feature_bits, "ProcessorSerialNumber", ((regs.edx & 0x40000) > 0));
	add_assoc_bool(feature_bits, "CLFLUSHInstruction", ((regs.edx & 0x80000) > 0));

	add_assoc_bool(feature_bits, "DebugStore", ((regs.edx & 0x200000) > 0));
	add_assoc_bool(feature_bits, "OnboardThermalControlMSRsForACPI", ((regs.edx & 0x400000) > 0));
	add_assoc_bool(feature_bits, "MMXInstructions", ((regs.edx & 0x800000) > 0));
	add_assoc_bool(feature_bits, "FXSAVE_FXRESTOR_Instructions", ((regs.edx & 0x1000000) > 0));
	add_assoc_bool(feature_bits, "SSEInstructions", ((regs.edx & 0x2000000) > 0));
	add_assoc_bool(feature_bits, "SSE2Instructions", ((regs.edx & 0x4000000) > 0));
	add_assoc_bool(feature_bits, "CPUCacheSupportsSelfSnoop", ((regs.edx & 0x8000000) > 0));
	add_assoc_bool(feature_bits, "HyperThreading", ((regs.edx & 0x10000000) > 0));
	add_assoc_bool(feature_bits, "ThermalMonitorAuomaticallyLimitsTemperature", ((regs.edx & 0x20000000) > 0));
	add_assoc_bool(feature_bits, "IA64ProcessorEmulatingx86", ((regs.edx & 0x40000000) > 0));
	add_assoc_bool(feature_bits, "PendingBreakEnableWakeupSupport", ((regs.edx & 0x80000000) > 0));
}

ZEND_METHOD(LibCpuId, detectExtendedProcessorInfoAndFeatureBits) {
	struct CpuRegisters regs = cpuid(0x80000001);
	zval *feature_bits;

	feature_bits = zend_read_property(ce_libcpuid, getThis(), "_featureBits", strlen("_featureBits"), 0 TSRMLS_CC);

	add_assoc_bool(feature_bits, "LAHF_SAHF_InLongMode", ((regs.ecx & 0x01) > 0));
	add_assoc_bool(feature_bits, "HyperThreadingNotValid", ((regs.ecx & 0x02) > 0));
	add_assoc_bool(feature_bits, "SecureVirtualMachine", ((regs.ecx & 0x04) > 0));
	add_assoc_bool(feature_bits, "ExtendedAPICSpace", ((regs.ecx & 0x08) > 0));
	add_assoc_bool(feature_bits, "CR8In32BitMode", ((regs.ecx & 0x10) > 0));
	add_assoc_bool(feature_bits, "AdvancedBitManipulation", ((regs.ecx & 0x20) > 0));
	add_assoc_bool(feature_bits, "SSE4a", ((regs.ecx & 0x40) > 0));
	add_assoc_bool(feature_bits, "MisalignedSSEMode", ((regs.ecx & 0x80) > 0));
	add_assoc_bool(feature_bits, "PREFETCHAndPREFETCHWInstructions", ((regs.ecx & 0x100) > 0));
	add_assoc_bool(feature_bits, "OSVisibleWorkaround", ((regs.ecx & 0x200) > 0));
	add_assoc_bool(feature_bits, "InstructionBasedSampling", ((regs.ecx & 0x400) > 0));
	add_assoc_bool(feature_bits, "XOPInstructionSet", ((regs.ecx & 0x800) > 0));
	add_assoc_bool(feature_bits, "SKINIT_STGI_Instructions", ((regs.ecx & 0x1000) > 0));
	add_assoc_bool(feature_bits, "WatchdogTimer", ((regs.ecx & 0x2000) > 0));

	add_assoc_bool(feature_bits, "LightweightProfiling", ((regs.ecx & 0x8000) > 0));
	add_assoc_bool(feature_bits, "FourOperandsFusedMultiplyAdd", ((regs.ecx & 0x10000) > 0));
	add_assoc_bool(feature_bits, "TranslationCacheExtension", ((regs.ecx & 0x20000) > 0));

	add_assoc_bool(feature_bits, "NodeID_MSR", ((regs.ecx & 0x80000) > 0));

	add_assoc_bool(feature_bits, "TrailingBitManipulation", ((regs.ecx & 0x200000) > 0));
	add_assoc_bool(feature_bits, "TopologyExtensions", ((regs.ecx & 0x400000) > 0));
	add_assoc_bool(feature_bits, "CorePerformanceCounterExtensions", ((regs.ecx & 0x800000) > 0));
	add_assoc_bool(feature_bits, "NBPerformanceCounterExtensions", ((regs.ecx & 0x1000000) > 0));

	add_assoc_bool(feature_bits, "SYSCALLAndSYSRETInstructions", ((regs.edx & 0x800) > 0));

	add_assoc_bool(feature_bits, "MultiprocessorCapable", ((regs.edx & 0x80000) > 0));
	add_assoc_bool(feature_bits, "NXBit", ((regs.edx & 0x100000) > 0));

	add_assoc_bool(feature_bits, "ExtendedMMX", ((regs.edx & 0x400000) > 0));

	add_assoc_bool(feature_bits, "FXSAVE_FXRSTOR_Instructions", ((regs.edx & 0x1000000) > 0));
	add_assoc_bool(feature_bits, "FXSAVE_FXRSTOR_Optimizations", ((regs.edx & 0x2000000) > 0));
	add_assoc_bool(feature_bits, "GibibytePages", ((regs.edx & 0x4000000) > 0));
	add_assoc_bool(feature_bits, "RDTSCPInstruction", ((regs.edx & 0x8000000) > 0));

	add_assoc_bool(feature_bits, "LongMode", ((regs.edx & 0x20000000) > 0));
	add_assoc_bool(feature_bits, "Extended3DNow", ((regs.edx & 0x40000000) > 0));
	add_assoc_bool(feature_bits, "AMD3DNow", ((regs.edx & 0x80000000) > 0));
}
