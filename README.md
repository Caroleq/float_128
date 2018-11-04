Following implementation of a number consists of:
- 1-bit sign
- 13-bit exponent 
- 114-bit mantissa 

Formula to compute decimal number:
\begin{equation*}

x  = (-1)^s 2^{E-bias}(1+M)

Where:
s - sign
E - exponent
M - mantissa
bias = 4095
\end{equation*}

Range (approximately):
\begin{equation*}

[-1.044389 \times 10^1233, -1.914996 \times 10^{-1233} ] \cup [1.914996 \times 10^{-1233},  1.044389 \times 10^1233]

\end{equation*}
