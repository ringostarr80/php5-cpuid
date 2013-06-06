--TEST--
cpuid() function - basic test for cpuid()
--SKIPIF--
<?php
if (!extension_loaded('cpuid')) print 'skip cpuid extension not available';
?>
--FILE--
<?php
$regs = cpuid(0);
var_dump(count($regs));
?>
--EXPECT--
int(4)