EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ADUC816 U1
U 1 1 58214753
P 4600 3700
F 0 "U1" H 3650 5500 50  0000 L CNN
F 1 "ADUC816" H 3650 5400 50  0000 L CNN
F 2 "Housings_QFP:LQFP-52_10x10mm_Pitch0.65mm" H 4600 3500 50  0001 C CNN
F 3 "" H 4600 3500 50  0000 C CNN
	1    4600 3700
	1    0    0    1   
$EndComp
$Comp
L C C1
U 1 1 582147FC
P 6100 3150
F 0 "C1" H 6125 3250 50  0000 L CNN
F 1 "C" H 6125 3050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 6138 3000 50  0001 C CNN
F 3 "" H 6100 3150 50  0000 C CNN
	1    6100 3150
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 582148E3
P 6100 3600
F 0 "C2" H 6125 3700 50  0000 L CNN
F 1 "C" H 6125 3500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0402" H 6138 3450 50  0001 C CNN
F 3 "" H 6100 3600 50  0000 C CNN
	1    6100 3600
	1    0    0    -1  
$EndComp
$Comp
L C C4
U 1 1 58214929
P 6400 3600
F 0 "C4" H 6425 3700 50  0000 L CNN
F 1 "C" H 6425 3500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6438 3450 50  0001 C CNN
F 3 "" H 6400 3600 50  0000 C CNN
	1    6400 3600
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 58214957
P 6400 3150
F 0 "C3" H 6425 3250 50  0000 L CNN
F 1 "C" H 6425 3050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6438 3000 50  0001 C CNN
F 3 "" H 6400 3150 50  0000 C CNN
	1    6400 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 3100 5850 3100
Wire Wire Line
	5850 3100 5850 3000
Wire Wire Line
	5850 3000 6400 3000
Connection ~ 6100 3000
Wire Wire Line
	5700 3300 6700 3300
Connection ~ 6100 3300
Wire Wire Line
	5700 3750 6700 3750
Wire Wire Line
	5700 3750 5700 3700
Connection ~ 6100 3750
Wire Wire Line
	5700 3600 5700 3450
Wire Wire Line
	5700 3450 6400 3450
Connection ~ 6100 3450
$Comp
L R R1
U 1 1 58214B73
P 6700 3450
F 0 "R1" V 6780 3450 50  0000 C CNN
F 1 "R" V 6700 3450 50  0000 C CNN
F 2 "Capacitors_SMD:C_0603" V 6630 3450 50  0001 C CNN
F 3 "" H 6700 3450 50  0000 C CNN
	1    6700 3450
	1    0    0    -1  
$EndComp
Connection ~ 6400 3300
Wire Wire Line
	6700 3750 6700 3600
Connection ~ 6400 3750
$EndSCHEMATC
