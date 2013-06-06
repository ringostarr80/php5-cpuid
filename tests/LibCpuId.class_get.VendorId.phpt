--TEST--
Class LibCpuId() - basic test for new LibCpuId()
--SKIPIF--
<?php
if (!extension_loaded('cpuid')) print 'skip cpuid extension not available';
?>
--FILE--
<?php
$libCpuId = new LibCpuId();
var_dump(strlen($libCpuId->VendorId) >= 10);
?>
--EXPECT--
bool(true)