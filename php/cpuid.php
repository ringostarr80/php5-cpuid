<?php
require_once 'LibCpuId.class.php';
// if (php_sapi_name() == 'cli')
if (PHP_SAPI == 'cli') {
	if (!extension_loaded('cpuid')) {
		print 'PHP Extension cpuid is not loaded!';
		return 1;
	} // if (!extension_loaded('cpuid'))

	$libCpuId = new LibCpuId();
	print ' eax in    eax      ebx      ecx      edx'."\n";
	foreach($libCpuId->RawData as $eaxInput => $rawRegisters) {
		print sprintf("%08x %08x %08x %08x %08x\n", $eaxInput, $rawRegisters['eax'], $rawRegisters['ebx'], $rawRegisters['ecx'], $rawRegisters['edx']);
	} // foreach($libCpuId->RawData as $eaxInput => $rawRegisters)

	print "\n";
	print sprintf("Vendor ID: \"%s\"; CPUID level %d\n", $libCpuId->VendorId, $libCpuId->CpuIdLevel);
	print "\n";

	print sprintf("Intel-specific functions:\n");
	print sprintf("Version %08x:\n", $libCpuId->ProcessorInfo['Raw']);
	print sprintf("Type %d - \n", $libCpuId->ProcessorInfo['ProcessorType']);
	print sprintf("Family %d - \n", $libCpuId->ProcessorInfo['Family']);
	print sprintf("Model %d - \n", $libCpuId->ProcessorInfo['Model']);
	print sprintf("Stepping %d\n", $libCpuId->ProcessorInfo['Stepping']);
	print sprintf("Reserved\n");
	print "\n";
	print sprintf("Extended brand string: \"%s\"\n", $libCpuId->ProcessorBrandString);
	print "\n";
	print sprintf("Feature flags %08x:", $libCpuId->RawData[1]['ebx']|$libCpuId->RawData[1]['ecx']|$libCpuId->RawData[1]['edx']);
	print "\n";
} else {
	if (!extension_loaded('cpuid')) {
		print 'PHP Extension cpuid is not loaded!';
		return;
	} // if (!extension_loaded('cpuid'))

	$libCpuId = new LibCpuId();
	?>
<!DOCTYPE html>
<html>
	<head>
		<title>cpuid informations in php</title>
	</head>
	<body>
		<h1>cpuid demonstration</h1>
		<table>
			<caption>raw data</caption>
			<thead><tr><th>eax in</th><th>eax</th><th>ebx</th><th>ecx</th><th>edx</th></tr></thead>
			<tbody>
				<?php
				foreach($libCpuId->RawData as $eaxInput => $rawRegisters) {
					print sprintf("<tr><td>%08x</td><td>%08x</td><td>%08x</td><td>%08x</td><td>%08x</td>\n", $eaxInput, $rawRegisters['eax'], $rawRegisters['ebx'], $rawRegisters['ecx'], $rawRegisters['edx']);
				} // foreach($libCpuId->RawData as $eaxInput => $rawRegisters)
				?>
			</tbody>
		</table>
		<p><?php print sprintf("Vendor ID: \"%s\"; CPUID level %d\n", $libCpuId->VendorId, $libCpuId->CpuIdLevel); ?></p>
		<p>
			<?php
			print sprintf("Intel-specific functions:<br />\n");
			print sprintf("Version %08x:<br />\n", $libCpuId->ProcessorInfo['Raw']);
			print sprintf("Type %d - <br />\n", $libCpuId->ProcessorInfo['ProcessorType']);
			print sprintf("Family %d - <br />\n", $libCpuId->ProcessorInfo['Family']);
			print sprintf("Model %d - <br />\n", $libCpuId->ProcessorInfo['Model']);
			print sprintf("Stepping %d<br />\n", $libCpuId->ProcessorInfo['Stepping']);
			print sprintf("Reserved\n");
			?>
		</p>
		<p><?php print sprintf("Extended brand string: \"%s\"\n", $libCpuId->ProcessorBrandString); ?></p>
		<p><?php print sprintf("Feature flags %08x:", $libCpuId->RawData[1]['ebx']|$libCpuId->RawData[1]['ecx']|$libCpuId->RawData[1]['edx']); ?></p>
	</body>
</html>
	<?php
} // if (PHP_SAPI == 'cli')
?>
