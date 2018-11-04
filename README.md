Following implementation of a number consists of:
- 1-bit sign
- 13-bit exponent 
- 114-bit mantissa 

Formula to compute decimal number:
x  = (-1)^s 2^{E-bias}(1+M)

Where:
 - s - sign
 - E - exponent
 - M - mantissa
 - bias = 4095


