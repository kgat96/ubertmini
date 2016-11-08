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
LIBS:ubertooth-symbols
LIBS:t3-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "ubertooth pro"
Date "2016-11-08"
Rev "1.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CC2400 U?
U 1 1 58218126
P 4550 2600
F 0 "U?" H 4550 2700 60  0000 C CNN
F 1 "CC2400" H 4550 2500 60  0000 C CNN
F 2 "" H 4550 2600 60  0001 C CNN
F 3 "" H 4550 2600 60  0001 C CNN
	1    4550 2600
	1    0    0    -1  
$EndComp
$Comp
L ADUC816 U?
U 1 1 582182A6
P 9600 2750
F 0 "U?" H 8650 4550 50  0000 L CNN
F 1 "ADUC816" H 8650 4450 50  0000 L CNN
F 2 "" H 9600 2550 50  0000 C CNN
F 3 "" H 9600 2550 50  0000 C CNN
	1    9600 2750
	1    0    0    -1  
$EndComp
$Comp
L L_Small L?
U 1 1 5821BE66
P 2900 2650
F 0 "L?" H 2950 2700 50  0000 L CNN
F 1 "L_Small" H 2930 2610 50  0000 L CNN
F 2 "" H 2900 2650 50  0000 C CNN
F 3 "" H 2900 2650 50  0000 C CNN
	1    2900 2650
	1    0    0    -1  
$EndComp
$Comp
L CC2591 U?
U 1 1 5821C144
P 1750 2700
F 0 "U?" H 1750 2800 60  0000 C CNN
F 1 "CC2591" H 1750 2600 60  0000 C CNN
F 2 "" H 1750 2700 60  0001 C CNN
F 3 "" H 1750 2700 60  0001 C CNN
	1    1750 2700
	-1   0    0    1   
$EndComp
Wire Wire Line
	2650 2650 3200 2650
Wire Wire Line
	2650 2550 3200 2550
Connection ~ 2900 2550
Wire Wire Line
	2650 2750 3200 2750
Connection ~ 2900 2750
$EndSCHEMATC
