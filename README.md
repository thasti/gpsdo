# GPS disciplined oscillator

For project description, see home page: http://loetlabor-jena.de/doku.php?id=projekte:gpsdo:start

A MSP430 capture timer is used in conjunction with the PPS-Pin of a GPS module to correct an OCXO frequency to its nominal value. An OCXO offers great stability but can not guarantee absolute frequency accuracy. The frequency is counted by gating with the PPS-signal for a configurable amount of time.

The frequency deviation is calculated and output on the debug UART. The output PWM (control voltage) is corrected according to the measured deviation. LEDs indicate the Lock-Status of the FLL.

