--TEST--
Class LibCpuId() - basic test for new LibCpuId()
--SKIPIF--
<?php
if (!extension_loaded('cpuid')) print 'skip cpuid extension not available';
?>
--FILE--
<?php
$libCpuId = new LibCpuId();
var_dump($libCpuId->CpuIdLevel > 1);
?>
--EXPECT--
bool(true)