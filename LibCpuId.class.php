<?php
/**
 * prototype declarations for the php5-cpuid extension
 * @author Ringo Leese <ringo19@gmx.de>
 */
if (!class_exists('LibCpuId')) {
	/**
	 * @property-read int $CpuIdLevel
	 * @property-read array $FeatureBits
	 * @property-read string $ProcessorBrandString
	 * @property-read array $ProcessorInfo
	 * @property-read array $RawData
	 * @property-read string $VendorId
	 */
	class LibCpuId
	{
		/**
		 * @access private
		 * @see $CpuIdLevel
		 * @var int
		 */
		private $_cpuidLevel = 0;
		/**
		 * @access private
		 * @see $FeatureBits
		 * @var array
		 */
		private $_featureBits = array();
		/**
		 * @access private
		 * @see $ProcessorBrandString
		 * @var string
		 */
		private $_processorBrandString = '';
		/**
		 * @access private
		 * @see $ProcessorInfo
		 * @var array
		 */
		private $_processorInfo = array();
		/**
		 * @access private
		 * @see $RawData
		 * @var array
		 */
		private $_rawData = array();
		/**
		 * @access private
		 * @see $VendorId
		 * @var string
		 */
		private $_vendorId = '';

		public function __construct() {

		} // __construct()

		/**
		 * @param string $name
		 */
		public function __get($name) {
			switch($name) {
				case 'CpuIdLevel':
					return $this->_cpuidLevel;
					break;

				case 'FeatureBits':
					return $this->_featureBits;
					break;

				case 'ProcessorBrandString':
					return $this->_processorBrandString;
					break;

				case 'ProcessorInfo':
					return $this->_processorInfo;
					break;

				case 'RawData':
					return $this->_rawData;
					break;

				case 'VendorId':
					return $this->_vendorId;
					break;
			}// switch($name)
		} // __get($name)
	} // class LibCpuId
} // if (!class_exists('LibCpuId'))

if (!function_exists('cpuid')) {
	/**
	 * @param int $eax_input
	 * @return array
	 */
	function cpuid($eax_input) {
		$eax_input = null;
		return array('eax' => 0, 'ebx' => 0, 'ecx' => 0, 'edx' => 0);
	} // cpuid($eax_input)
} // if (!function_exists('cpuid'))
?>