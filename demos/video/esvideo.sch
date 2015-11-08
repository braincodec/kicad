EESchema Schematic File Version 2
LIBS:video_schlib
LIBS:power
LIBS:video-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 8 8
Title "Video"
Date "Sun 22 Mar 2015"
Rev "2.0B"
Comp "Kicad EDA"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
NoConn ~ 11350 6250
Text Label 5700 2200 0    60   ~ 0
VAA
Wire Wire Line
	5650 2300 5650 2200
Wire Wire Line
	11350 6450 11900 6450
Wire Wire Line
	6550 7700 7150 7700
Text Label 6650 7700 0    60   ~ 0
OE_RVB-
Text Label 11450 6450 0    60   ~ 0
OE_RVB-
Wire Wire Line
	11350 6350 12000 6350
Text HLabel 12000 6350 2    60   Output ~ 0
OE_PAL-
Text Label 8950 6950 0    60   ~ 0
PCA2
Text Label 8950 6850 0    60   ~ 0
PCA1
Text Label 8950 6750 0    60   ~ 0
PCA0
Text Label 4100 7200 0    60   ~ 0
PCA2
Text Label 4100 7100 0    60   ~ 0
PCA1
Text Label 4100 7000 0    60   ~ 0
PCA0
$Comp
L GND #PWR0162
U 1 1 33A566EB
P 9300 7800
F 0 "#PWR0162" H 9300 7800 40  0001 C CNN
F 1 "GND" H 9300 7730 40  0001 C CNN
F 2 "" H 9300 7800 60  0001 C CNN
F 3 "" H 9300 7800 60  0001 C CNN
	1    9300 7800
	1    0    0    -1  
$EndComp
Connection ~ 4350 3100
$Comp
L GND #PWR0163
U 1 1 3392D11C
P 4350 4200
F 0 "#PWR0163" H 4350 4200 40  0001 C CNN
F 1 "GND" H 4350 4130 40  0001 C CNN
F 2 "" H 4350 4200 60  0001 C CNN
F 3 "" H 4350 4200 60  0001 C CNN
	1    4350 4200
	0    1    1    0   
$EndComp
Connection ~ 4350 3600
Connection ~ 4350 3500
Connection ~ 4350 3400
Connection ~ 4350 3300
Connection ~ 4350 3200
Wire Wire Line
	4350 3600 4450 3600
Wire Wire Line
	4350 3500 4450 3500
Wire Wire Line
	4350 3400 4450 3400
Wire Wire Line
	4350 3300 4450 3300
Wire Wire Line
	4450 3200 4350 3200
Wire Wire Line
	4350 3100 4350 3800
Wire Wire Line
	4350 3800 4450 3800
Wire Wire Line
	4350 4200 4450 4200
Text HLabel 1700 6200 0    60   Input ~ 0
BLUE_IN
Text HLabel 1700 5400 0    60   Input ~ 0
GREEN_IN
Text HLabel 1700 4600 0    60   Input ~ 0
RED_IN
$Comp
L GND #PWR0164
U 1 1 335F5DFC
P 12050 5050
F 0 "#PWR0164" H 12050 5050 40  0001 C CNN
F 1 "GND" H 12050 4980 40  0001 C CNN
F 2 "" H 12050 5050 60  0001 C CNN
F 3 "" H 12050 5050 60  0001 C CNN
	1    12050 5050
	0    -1   -1   0   
$EndComp
$Comp
L C C32
U 1 1 335F5DF2
P 11800 5050
F 0 "C32" V 11650 5050 50  0000 C CNN
F 1 "100nF" V 11950 5050 50  0000 C CNN
F 2 "lib_smd:SM1206" H 11800 5050 60  0001 C CNN
F 3 "" H 11800 5050 60  0001 C CNN
	1    11800 5050
	0    1    1    0   
$EndComp
Connection ~ 11450 4650
Connection ~ 11500 5050
Wire Wire Line
	11350 5050 11600 5050
Wire Wire Line
	11500 5050 11500 4450
Wire Wire Line
	11500 4450 11350 4450
Wire Wire Line
	11450 4850 11350 4850
Wire Wire Line
	11450 3300 11450 4850
Wire Wire Line
	9350 7550 8950 7550
Wire Wire Line
	9350 7450 8950 7450
Text Label 9000 7550 0    60   ~ 0
TVI1
Text Label 9000 7450 0    60   ~ 0
TVI0
Wire Wire Line
	6550 4100 6550 4000
Wire Wire Line
	6550 4000 6650 4000
NoConn ~ 4450 4900
Text HLabel 1500 750  0    60   Input ~ 0
TVI[0..1]
Wire Wire Line
	8900 5650 9350 5650
Wire Wire Line
	8900 5550 9350 5550
Wire Wire Line
	8900 5450 9350 5450
Wire Wire Line
	8900 5350 9350 5350
Wire Wire Line
	8900 5250 9350 5250
Wire Wire Line
	8900 5150 9350 5150
Wire Wire Line
	8900 5050 9350 5050
Wire Wire Line
	8900 4950 9350 4950
Wire Wire Line
	8900 4750 9350 4750
Wire Wire Line
	8900 4650 9350 4650
Wire Wire Line
	8900 4550 9350 4550
Wire Wire Line
	8900 4450 9350 4450
Wire Wire Line
	8900 4350 9350 4350
Wire Wire Line
	8900 4250 9350 4250
Wire Wire Line
	8900 4150 9350 4150
Wire Wire Line
	8900 4050 9350 4050
Wire Wire Line
	8900 3850 9350 3850
Wire Wire Line
	8900 3750 9350 3750
Wire Wire Line
	8900 3650 9350 3650
Wire Wire Line
	8900 3550 9350 3550
Wire Wire Line
	8900 3450 9350 3450
Wire Wire Line
	8900 3350 9350 3350
Wire Wire Line
	8900 3250 9350 3250
Wire Wire Line
	8900 3150 9350 3150
Text Label 9000 5650 0    60   ~ 0
TVB7
Text Label 9000 5550 0    60   ~ 0
TVB6
Text Label 9000 5450 0    60   ~ 0
TVB5
Text Label 9000 5350 0    60   ~ 0
TVB4
Text Label 9000 5250 0    60   ~ 0
TVB3
Text Label 9000 5150 0    60   ~ 0
TVB2
Text Label 9000 5050 0    60   ~ 0
TVB1
Text Label 9000 4950 0    60   ~ 0
TVB0
Text Label 9000 4750 0    60   ~ 0
TVG7
Text Label 9000 4650 0    60   ~ 0
TVG6
Text Label 9000 4550 0    60   ~ 0
TVG5
Text Label 9000 4450 0    60   ~ 0
TVG4
Text Label 9000 4350 0    60   ~ 0
TVG3
Text Label 9000 4250 0    60   ~ 0
TVG2
Text Label 9000 4150 0    60   ~ 0
TVG1
Text Label 9000 4050 0    60   ~ 0
TVG0
Text Label 9000 3850 0    60   ~ 0
TVR7
Text Label 9000 3750 0    60   ~ 0
TVR6
Text Label 9000 3650 0    60   ~ 0
TVR5
Text Label 9000 3550 0    60   ~ 0
TVR4
Text Label 9000 3450 0    60   ~ 0
TVR3
Text Label 9000 3350 0    60   ~ 0
TVR2
Text Label 9000 3250 0    60   ~ 0
TVR1
Text Label 9000 3150 0    60   ~ 0
TVR0
Wire Wire Line
	6550 7500 7000 7500
Wire Wire Line
	6550 7400 7000 7400
Wire Wire Line
	6550 7300 7000 7300
Wire Wire Line
	6550 7200 7000 7200
Wire Wire Line
	6550 7100 7000 7100
Wire Wire Line
	6550 7000 7000 7000
Wire Wire Line
	6550 6900 7000 6900
Wire Wire Line
	6550 6800 7000 6800
Wire Wire Line
	6550 6600 7000 6600
Wire Wire Line
	6550 6500 7000 6500
Wire Wire Line
	6550 6400 7000 6400
Wire Wire Line
	6550 6300 7000 6300
Wire Wire Line
	6550 6200 7000 6200
Wire Wire Line
	6550 6100 7000 6100
Wire Wire Line
	6550 6000 7000 6000
Wire Wire Line
	6550 5900 7000 5900
Wire Wire Line
	6550 5700 7000 5700
Wire Wire Line
	6550 5600 7000 5600
Wire Wire Line
	6550 5500 7000 5500
Wire Wire Line
	6550 5400 7000 5400
Wire Wire Line
	6550 5300 7000 5300
Wire Wire Line
	6550 5200 7000 5200
Wire Wire Line
	6550 5100 7000 5100
Wire Wire Line
	6550 5000 7000 5000
Text Label 6650 7500 0    60   ~ 0
TVB7
Text Label 6650 7400 0    60   ~ 0
TVB6
Text Label 6650 7300 0    60   ~ 0
TVB5
Text Label 6650 7200 0    60   ~ 0
TVB4
Text Label 6650 7100 0    60   ~ 0
TVB3
Text Label 6650 7000 0    60   ~ 0
TVB2
Text Label 6650 6900 0    60   ~ 0
TVB1
Text Label 6650 6800 0    60   ~ 0
TVB0
Text Label 6650 6600 0    60   ~ 0
TVG7
Text Label 6650 6500 0    60   ~ 0
TVG6
Text Label 6650 6400 0    60   ~ 0
TVG5
Text Label 6650 6300 0    60   ~ 0
TVG4
Text Label 6650 6200 0    60   ~ 0
TVG3
Text Label 6650 6100 0    60   ~ 0
TVG2
Text Label 6650 6000 0    60   ~ 0
TVG1
Text Label 6650 5900 0    60   ~ 0
TVG0
Text Label 6650 5700 0    60   ~ 0
TVR7
Text Label 6650 5600 0    60   ~ 0
TVR6
Text Label 6650 5500 0    60   ~ 0
TVR5
Text Label 6650 5400 0    60   ~ 0
TVR4
Text Label 6650 5300 0    60   ~ 0
TVR3
Text Label 6650 5200 0    60   ~ 0
TVR2
Text Label 6650 5100 0    60   ~ 0
TVR1
Text Label 6650 5000 0    60   ~ 0
TVR0
Text HLabel 6550 2700 2    60   Output ~ 0
CSYNCIN-
Text HLabel 6650 4600 2    60   Input ~ 0
CLKCAD
Wire Wire Line
	6650 4600 6550 4600
$Comp
L GND #GND0165
U 1 1 A458682C
P 12850 4250
F 0 "#GND0165" H 12850 4350 60  0001 C CNN
F 1 "GND" H 12850 4150 60  0001 C CNN
F 2 "" H 12850 4250 60  0001 C CNN
F 3 "" H 12850 4250 60  0001 C CNN
	1    12850 4250
	0    -1   -1   0   
$EndComp
$Comp
L R R8
U 1 1 A4586827
P 12550 4250
F 0 "R8" V 12630 4250 50  0000 C CNN
F 1 "150" V 12550 4250 50  0000 C CNN
F 2 "lib_smd:SM1206" H 12550 4250 60  0001 C CNN
F 3 "" H 12550 4250 60  0001 C CNN
	1    12550 4250
	0    -1   -1   0   
$EndComp
Connection ~ 12150 4250
Wire Wire Line
	11350 4250 12300 4250
Wire Wire Line
	12150 2800 12150 4250
Wire Wire Line
	11450 4650 11350 4650
Text HLabel 1500 1250 0    60   3State ~ 0
DPC[0..7]
Text HLabel 1500 1350 0    60   Input ~ 0
PCA[0..2]
$Comp
L GND #GND0166
U 1 1 32FA0485
P 5650 8250
F 0 "#GND0166" H 5650 8250 40  0001 C CNN
F 1 "GND" H 5650 8180 40  0001 C CNN
F 2 "" H 5650 8250 60  0001 C CNN
F 3 "" H 5650 8250 60  0001 C CNN
	1    5650 8250
	1    0    0    -1  
$EndComp
Text HLabel 1500 1050 0    60   3State ~ 0
TVB[0..7]
Text HLabel 1500 950  0    60   3State ~ 0
TVG[0..7]
Text HLabel 1500 850  0    60   3State ~ 0
TVR[0..7]
Text HLabel 11400 5450 2    60   Input ~ 0
CSYNCOUT-
Text HLabel 11400 5650 2    60   Input ~ 0
BLANK-
Text HLabel 11400 5850 2    60   Input ~ 0
CLKCDA
Text HLabel 6650 4000 2    60   Input ~ 0
CLAMP
NoConn ~ 11350 6550
NoConn ~ 11350 7050
NoConn ~ 11350 6950
Wire Wire Line
	13150 3850 13150 3250
Wire Wire Line
	11350 3850 13450 3850
Wire Wire Line
	12900 3650 12900 3250
Wire Wire Line
	11350 3650 13450 3650
$Comp
L GND #GND0167
U 1 1 32F9F3E8
P 10450 8250
F 0 "#GND0167" H 10450 8250 40  0001 C CNN
F 1 "GND" H 10450 8180 40  0001 C CNN
F 2 "" H 10450 8250 60  0001 C CNN
F 3 "" H 10450 8250 60  0001 C CNN
	1    10450 8250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 6200 1800 6200
Wire Wire Line
	3800 2100 4450 2100
Wire Wire Line
	4450 2100 4450 2700
Wire Wire Line
	4350 2900 4450 2900
Wire Wire Line
	4350 2800 4450 2800
Connection ~ 6550 4700
Wire Wire Line
	6550 4600 6550 4800
Wire Wire Line
	6550 3700 6550 3800
Wire Wire Line
	6550 3400 6550 3500
Wire Wire Line
	6550 3100 6550 3200
Connection ~ 6550 4300
Wire Wire Line
	6550 4200 6550 4400
Wire Wire Line
	6550 4400 6650 4400
NoConn ~ 4450 5400
NoConn ~ 4450 5300
NoConn ~ 4450 5200
NoConn ~ 4450 5100
Connection ~ 4450 4300
Connection ~ 4450 3900
Wire Wire Line
	4450 4200 4450 4400
Wire Wire Line
	4450 3800 4450 4000
Text HLabel 9300 7250 0    60   Input ~ 0
WRCDA-
Text HLabel 9300 7150 0    60   Input ~ 0
RDCDA-
Connection ~ 10250 2750
Connection ~ 10150 2750
Wire Wire Line
	4050 7200 4450 7200
Text HLabel 4450 6800 0    60   Input ~ 0
WRCAD-
Text HLabel 4450 6700 0    60   Input ~ 0
RDCAD-
$Comp
L BT473 U9
U 1 1 32F9E902
P 10350 5450
F 0 "U9" H 10350 5550 70  0000 C CNN
F 1 "BT473" H 10350 5350 70  0000 C CNN
F 2 "footprints:PLCC68" H 10350 5450 60  0001 C CNN
F 3 "" H 10350 5450 60  0001 C CNN
	1    10350 5450
	1    0    0    -1  
$EndComp
$Comp
L BT253 U8
U 1 1 32F9E7F1
P 5500 5200
F 0 "U8" H 5500 5300 70  0000 C CNN
F 1 "BT253" H 5500 5100 70  0000 C CNN
F 2 "footprints:PLCC84" H 5500 5200 60  0001 C CNN
F 3 "" H 5500 5200 60  0001 C CNN
	1    5500 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 2000 3800 2200
Wire Wire Line
	3800 2800 3800 2600
Wire Wire Line
	3750 2800 3850 2800
Wire Wire Line
	2750 6200 3000 6200
Wire Wire Line
	2300 6200 2350 6200
Wire Wire Line
	2750 5400 2900 5400
Wire Wire Line
	2300 5400 2350 5400
Wire Wire Line
	2300 4600 2350 4600
Wire Wire Line
	2750 4600 4450 4600
Wire Wire Line
	1700 5400 1800 5400
Wire Wire Line
	1700 4600 1800 4600
$Comp
L R R42
U 1 1 84DFB9D2
P 12600 3000
F 0 "R42" H 12500 3200 50  0000 C CNN
F 1 "75" V 12600 3000 50  0000 C CNN
F 2 "lib_smd:SM1206" H 12600 3000 60  0001 C CNN
F 3 "" H 12600 3000 60  0001 C CNN
	1    12600 3000
	1    0    0    -1  
$EndComp
$Comp
L R R43
U 1 1 84DFB9D7
P 12900 3000
F 0 "R43" H 12800 3200 50  0000 C CNN
F 1 "75" V 12900 3000 50  0000 C CNN
F 2 "lib_smd:SM1206" H 12900 3000 60  0001 C CNN
F 3 "" H 12900 3000 60  0001 C CNN
	1    12900 3000
	1    0    0    -1  
$EndComp
$Comp
L R R44
U 1 1 84DFB9DC
P 13150 3000
F 0 "R44" H 13050 3200 50  0000 C CNN
F 1 "75" V 13150 3000 50  0000 C CNN
F 2 "lib_smd:SM1206" H 13150 3000 60  0001 C CNN
F 3 "" H 13150 3000 60  0001 C CNN
	1    13150 3000
	1    0    0    -1  
$EndComp
$Comp
L C C33
U 1 1 84DFBA31
P 9350 2500
F 0 "C33" H 9500 2600 50  0000 C CNN
F 1 "100nF" H 9500 2400 50  0000 C CNN
F 2 "lib_smd:SM1206" H 9350 2500 60  0001 C CNN
F 3 "" H 9350 2500 60  0001 C CNN
	1    9350 2500
	1    0    0    -1  
$EndComp
$Comp
L C C34
U 1 1 84DFBA36
P 11650 2600
F 0 "C34" H 11800 2700 50  0000 C CNN
F 1 "100nF" H 11800 2500 50  0000 C CNN
F 2 "lib_smd:SM1206" H 11650 2600 60  0001 C CNN
F 3 "" H 11650 2600 60  0001 C CNN
	1    11650 2600
	1    0    0    -1  
$EndComp
$Comp
L CP C42
U 1 1 84DFBAEF
P 12150 2600
F 0 "C42" H 12300 2700 50  0000 C CNN
F 1 "2.2uF" H 12300 2500 50  0000 C CNN
F 2 "lib_smd:SM1210L" H 12150 2600 60  0001 C CNN
F 3 "" H 12150 2600 60  0001 C CNN
	1    12150 2600
	1    0    0    -1  
$EndComp
$Comp
L C C35
U 1 1 84DFBB21
P 11450 3100
F 0 "C35" H 11600 3200 50  0000 C CNN
F 1 "100nF" H 11600 3000 50  0000 C CNN
F 2 "lib_smd:SM1206" H 11450 3100 60  0001 C CNN
F 3 "" H 11450 3100 60  0001 C CNN
	1    11450 3100
	1    0    0    -1  
$EndComp
$Comp
L GND #GND0168
U 1 1 684863B9
P 9350 2750
F 0 "#GND0168" H 9350 2850 60  0001 C CNN
F 1 "GND" H 9350 2650 60  0001 C CNN
F 2 "" H 9350 2750 60  0001 C CNN
F 3 "" H 9350 2750 60  0001 C CNN
	1    9350 2750
	1    0    0    -1  
$EndComp
$Comp
L GND #GND0169
U 1 1 684863BE
P 9850 2750
F 0 "#GND0169" H 9850 2850 60  0001 C CNN
F 1 "GND" H 9850 2650 60  0001 C CNN
F 2 "" H 9850 2750 60  0001 C CNN
F 3 "" H 9850 2750 60  0001 C CNN
	1    9850 2750
	1    0    0    -1  
$EndComp
$Comp
L GND #GND0170
U 1 1 684863C3
P 12600 2700
F 0 "#GND0170" H 12600 2800 60  0001 C CNN
F 1 "GND" H 12600 2600 60  0001 C CNN
F 2 "" H 12600 2700 60  0001 C CNN
F 3 "" H 12600 2700 60  0001 C CNN
	1    12600 2700
	-1   0    0    1   
$EndComp
$Comp
L GND #GND0171
U 1 1 684863C8
P 12900 2700
F 0 "#GND0171" H 12900 2800 60  0001 C CNN
F 1 "GND" H 12900 2600 60  0001 C CNN
F 2 "" H 12900 2700 60  0001 C CNN
F 3 "" H 12900 2700 60  0001 C CNN
	1    12900 2700
	-1   0    0    1   
$EndComp
$Comp
L GND #GND0172
U 1 1 684863CD
P 13150 2700
F 0 "#GND0172" H 13150 2800 60  0001 C CNN
F 1 "GND" H 13150 2600 60  0001 C CNN
F 2 "" H 13150 2700 60  0001 C CNN
F 3 "" H 13150 2700 60  0001 C CNN
	1    13150 2700
	-1   0    0    1   
$EndComp
$Comp
L GND #GND0173
U 1 1 A44C032A
P 6650 4400
F 0 "#GND0173" H 6650 4500 60  0001 C CNN
F 1 "GND" H 6650 4300 60  0000 C CNN
F 2 "" H 6650 4400 60  0001 C CNN
F 3 "" H 6650 4400 60  0001 C CNN
	1    6650 4400
	0    -1   -1   0   
$EndComp
$Comp
L R R47
U 1 1 A44C0348
P 2050 5000
F 0 "R47" V 2130 5000 50  0000 C CNN
F 1 "75" V 2050 5000 50  0000 C CNN
F 2 "lib_smd:SM1206" H 2050 5000 60  0001 C CNN
F 3 "" H 2050 5000 60  0001 C CNN
	1    2050 5000
	0    -1   -1   0   
$EndComp
$Comp
L R R45
U 1 1 A44C032F
P 2050 5800
F 0 "R45" V 2130 5800 50  0000 C CNN
F 1 "75" V 2050 5800 50  0000 C CNN
F 2 "lib_smd:SM1206" H 2050 5800 60  0001 C CNN
F 3 "" H 2050 5800 60  0001 C CNN
	1    2050 5800
	0    -1   -1   0   
$EndComp
$Comp
L R R46
U 1 1 A44C0339
P 2050 6600
F 0 "R46" V 2130 6600 50  0000 C CNN
F 1 "75" V 2050 6600 50  0000 C CNN
F 2 "lib_smd:SM1206" H 2050 6600 60  0001 C CNN
F 3 "" H 2050 6600 60  0001 C CNN
	1    2050 6600
	0    -1   -1   0   
$EndComp
$Comp
L GND #GND0174
U 1 1 A44C0393
P 2350 5000
F 0 "#GND0174" H 2350 5100 60  0001 C CNN
F 1 "GND" H 2350 4900 60  0001 C CNN
F 2 "" H 2350 5000 60  0001 C CNN
F 3 "" H 2350 5000 60  0001 C CNN
	1    2350 5000
	0    -1   -1   0   
$EndComp
$Comp
L GND #GND0175
U 1 1 A44C0398
P 2350 5800
F 0 "#GND0175" H 2350 5900 60  0001 C CNN
F 1 "GND" H 2350 5700 60  0001 C CNN
F 2 "" H 2350 5800 60  0001 C CNN
F 3 "" H 2350 5800 60  0001 C CNN
	1    2350 5800
	0    -1   -1   0   
$EndComp
$Comp
L GND #GND0176
U 1 1 A44C039D
P 2350 6600
F 0 "#GND0176" H 2350 6700 60  0001 C CNN
F 1 "GND" H 2350 6500 60  0001 C CNN
F 2 "" H 2350 6600 60  0001 C CNN
F 3 "" H 2350 6600 60  0001 C CNN
	1    2350 6600
	0    -1   -1   0   
$EndComp
$Comp
L R R38
U 1 1 A44C03AC
P 4100 2900
F 0 "R38" V 4180 2900 50  0000 C CNN
F 1 "510" V 4100 2900 50  0000 C CNN
F 2 "lib_smd:SM1206" H 4100 2900 60  0001 C CNN
F 3 "" H 4100 2900 60  0001 C CNN
	1    4100 2900
	0    1    1    0   
$EndComp
$Comp
L GND #GND0177
U 1 1 A44C03B1
P 3800 2900
F 0 "#GND0177" H 3800 3000 60  0001 C CNN
F 1 "GND" H 3800 2800 60  0001 C CNN
F 2 "" H 3800 2900 60  0001 C CNN
F 3 "" H 3800 2900 60  0001 C CNN
	1    3800 2900
	0    1    1    0   
$EndComp
$Comp
L GND #GND0178
U 1 1 A44D982F
P 3800 1450
F 0 "#GND0178" H 3800 1550 60  0001 C CNN
F 1 "GND" H 3800 1350 60  0001 C CNN
F 2 "" H 3800 1450 60  0001 C CNN
F 3 "" H 3800 1450 60  0001 C CNN
	1    3800 1450
	-1   0    0    1   
$EndComp
$Comp
L R R15
U 1 1 A44D982A
P 3800 1750
F 0 "R15" V 3880 1750 50  0000 C CNN
F 1 "1M" V 3800 1750 50  0000 C CNN
F 2 "lib_smd:SM1206" H 3800 1750 60  0001 C CNN
F 3 "" H 3800 1750 60  0001 C CNN
	1    3800 1750
	1    0    0    -1  
$EndComp
$Comp
L GND #GND0179
U 1 1 A44D9852
P 3300 2800
F 0 "#GND0179" H 3300 2900 60  0001 C CNN
F 1 "GND" H 3300 2700 60  0001 C CNN
F 2 "" H 3300 2800 60  0001 C CNN
F 3 "" H 3300 2800 60  0001 C CNN
	1    3300 2800
	0    1    1    0   
$EndComp
$Comp
L C C36
U 1 1 A44D9848
P 3800 2400
F 0 "C36" H 3950 2500 50  0000 C CNN
F 1 "100nF" H 3950 2300 50  0000 C CNN
F 2 "lib_smd:SM1206" H 3800 2400 60  0001 C CNN
F 3 "" H 3800 2400 60  0001 C CNN
	1    3800 2400
	1    0    0    -1  
$EndComp
$Comp
L R R36
U 1 1 A44D9843
P 4100 2800
F 0 "R36" V 4180 2800 50  0000 C CNN
F 1 "470" V 4100 2800 50  0000 C CNN
F 2 "lib_smd:SM1206" H 4100 2800 60  0001 C CNN
F 3 "" H 4100 2800 60  0001 C CNN
	1    4100 2800
	0    -1   -1   0   
$EndComp
$Comp
L C C37
U 1 1 A44D984D
P 3550 2800
F 0 "C37" V 3800 2800 50  0000 C CNN
F 1 "100pF" V 3700 2800 50  0000 C CNN
F 2 "lib_smd:SM1206" H 3550 2800 60  0001 C CNN
F 3 "" H 3550 2800 60  0001 C CNN
	1    3550 2800
	0    -1   -1   0   
$EndComp
$Comp
L R R24
U 1 1 A44C034D
P 2050 4600
F 0 "R24" V 2130 4600 50  0000 C CNN
F 1 "220" V 2050 4600 50  0000 C CNN
F 2 "lib_smd:SM1206" H 2050 4600 60  0001 C CNN
F 3 "" H 2050 4600 60  0001 C CNN
	1    2050 4600
	0    -1   -1   0   
$EndComp
$Comp
L R R23
U 1 1 A44C0343
P 2050 5400
F 0 "R23" V 2130 5400 50  0000 C CNN
F 1 "220" V 2050 5400 50  0000 C CNN
F 2 "lib_smd:SM1206" H 2050 5400 60  0001 C CNN
F 3 "" H 2050 5400 60  0001 C CNN
	1    2050 5400
	0    -1   -1   0   
$EndComp
$Comp
L R R22
U 1 1 A44C0334
P 2050 6200
F 0 "R22" V 2130 6200 50  0000 C CNN
F 1 "220" V 2050 6200 50  0000 C CNN
F 2 "lib_smd:SM1206" H 2050 6200 60  0001 C CNN
F 3 "" H 2050 6200 60  0001 C CNN
	1    2050 6200
	0    -1   -1   0   
$EndComp
$Comp
L C C39
U 1 1 A44C037F
P 2550 4600
F 0 "C39" V 2400 4600 50  0000 C CNN
F 1 "1uF" V 2700 4600 50  0000 C CNN
F 2 "lib_smd:SM1210L" H 2550 4600 60  0001 C CNN
F 3 "" H 2550 4600 60  0001 C CNN
	1    2550 4600
	0    1    1    0   
$EndComp
$Comp
L C C40
U 1 1 A44C0384
P 2550 5400
F 0 "C40" V 2400 5400 50  0000 C CNN
F 1 "1uF" V 2700 5400 50  0000 C CNN
F 2 "lib_smd:SM1210L" H 2550 5400 60  0001 C CNN
F 3 "" H 2550 5400 60  0001 C CNN
	1    2550 5400
	0    1    1    0   
$EndComp
$Comp
L C C41
U 1 1 A44C0389
P 2550 6200
F 0 "C41" V 2400 6200 50  0000 C CNN
F 1 "1uF" V 2700 6200 50  0000 C CNN
F 2 "lib_smd:SM1210L" H 2550 6200 60  0001 C CNN
F 3 "" H 2550 6200 60  0001 C CNN
	1    2550 6200
	0    1    1    0   
$EndComp
$Comp
L GND #GND0180
U 1 1 26FC0C80
P 6400 2200
F 0 "#GND0180" H 6400 2300 60  0001 C CNN
F 1 "GND" H 6400 2100 60  0001 C CNN
F 2 "" H 6400 2200 60  0001 C CNN
F 3 "" H 6400 2200 60  0001 C CNN
	1    6400 2200
	0    -1   -1   0   
$EndComp
$Comp
L CP C52
U 1 1 84DFB9B9
P 6150 2200
F 0 "C52" V 6300 2200 50  0000 C CNN
F 1 "22uF" V 6000 2200 50  0000 C CNN
F 2 "lib_smd:SM1206" H 6150 2200 60  0001 C CNN
F 3 "" H 6150 2200 60  0001 C CNN
	1    6150 2200
	0    -1   -1   0   
$EndComp
$Comp
L CP C51
U 1 1 28ED6A43
P 9850 2500
F 0 "C51" H 10000 2600 50  0000 C CNN
F 1 "22uF" H 10000 2400 50  0000 C CNN
F 2 "lib_smd:SM1210L" H 9850 2500 60  0001 C CNN
F 3 "" H 9850 2500 60  0001 C CNN
	1    9850 2500
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR L5
U 1 1 84DFBB99
P 12150 1800
F 0 "L5" H 12350 1850 50  0000 C CNN
F 1 "22uH" H 12350 1750 50  0000 C CNN
F 2 "lib_smd:SM1812" H 12150 1800 60  0001 C CNN
F 3 "" H 12150 1800 60  0001 C CNN
	1    12150 1800
	1    0    0    -1  
$EndComp
$Comp
L R R37
U 1 1 5D7688E4
P 3700 3100
F 0 "R37" V 3780 3100 50  0000 C CNN
F 1 "510" V 3700 3100 50  0000 C CNN
F 2 "lib_smd:SM1206" H 3700 3100 60  0001 C CNN
F 3 "" H 3700 3100 60  0001 C CNN
	1    3700 3100
	0    -1   -1   0   
$EndComp
$Comp
L GND #GND0181
U 1 1 5D7688FD
P 3400 3100
F 0 "#GND0181" H 3400 3200 60  0001 C CNN
F 1 "GND" H 3400 3000 60  0001 C CNN
F 2 "" H 3400 3100 60  0001 C CNN
F 3 "" H 3400 3100 60  0001 C CNN
	1    3400 3100
	0    1    1    0   
$EndComp
Text Label 10550 2200 0    60   ~ 0
VAA
Text HLabel 13450 3450 2    60   Output ~ 0
RED
Text HLabel 13450 3650 2    60   Output ~ 0
GREEN
Text HLabel 13450 3850 2    60   Output ~ 0
BLUE
Text Label 4050 5800 0    60   ~ 0
DPC0
Text Label 4050 5900 0    60   ~ 0
DPC1
Text Label 4050 6000 0    60   ~ 0
DPC2
Text Label 4050 6100 0    60   ~ 0
DPC3
Text Label 4050 6200 0    60   ~ 0
DPC4
Text Label 4050 6300 0    60   ~ 0
DPC5
Text Label 4050 6400 0    60   ~ 0
DPC6
Text Label 4050 6500 0    60   ~ 0
DPC7
Text Label 8950 5850 0    60   ~ 0
DPC0
Text Label 8950 5950 0    60   ~ 0
DPC1
Text Label 8950 6050 0    60   ~ 0
DPC2
Text Label 8950 6150 0    60   ~ 0
DPC3
Text Label 8950 6250 0    60   ~ 0
DPC4
Text Label 8950 6350 0    60   ~ 0
DPC5
Text Label 8950 6450 0    60   ~ 0
DPC6
Text Label 8950 6550 0    60   ~ 0
DPC7
Text Label 4000 3100 0    60   ~ 0
REF+
$Comp
L VCC #VCC0182
U 1 1 4BF036B4
P 12150 1450
F 0 "#VCC0182" H 12150 1650 40  0001 C CNN
F 1 "VCC" H 12150 1600 40  0000 C CNN
F 2 "" H 12150 1450 60  0001 C CNN
F 3 "" H 12150 1450 60  0001 C CNN
	1    12150 1450
	1    0    0    -1  
$EndComp
Connection ~ 9850 2200
Connection ~ 10550 2200
Connection ~ 11450 2200
Connection ~ 11650 2200
Connection ~ 10350 2750
Connection ~ 10450 2750
Connection ~ 12600 3450
Connection ~ 12900 3650
Connection ~ 13150 3850
Wire Wire Line
	5350 2200 5950 2200
Wire Wire Line
	9350 2200 12150 2200
Wire Wire Line
	10050 2750 10550 2750
Wire Wire Line
	11350 3450 13450 3450
Wire Wire Line
	3950 3100 4450 3100
Wire Wire Line
	4450 4700 2900 4700
Wire Wire Line
	8950 5850 9350 5850
Wire Wire Line
	4450 4800 3000 4800
Wire Wire Line
	8950 5950 9350 5950
Wire Wire Line
	8950 6050 9350 6050
Wire Wire Line
	8950 6150 9350 6150
Wire Wire Line
	4050 5800 4450 5800
Wire Wire Line
	8950 6250 9350 6250
Wire Wire Line
	4050 5900 4450 5900
Wire Wire Line
	8950 6350 9350 6350
Wire Wire Line
	4050 6000 4450 6000
Wire Wire Line
	8950 6450 9350 6450
Wire Wire Line
	4050 6100 4450 6100
Wire Wire Line
	8950 6550 9350 6550
Wire Wire Line
	4050 6200 4450 6200
Wire Wire Line
	4050 6300 4450 6300
Wire Wire Line
	8950 6750 9350 6750
Wire Wire Line
	4050 6400 4450 6400
Wire Wire Line
	8950 6850 9350 6850
Wire Wire Line
	4050 6500 4450 6500
Wire Wire Line
	8950 6950 9350 6950
Wire Wire Line
	4050 7000 4450 7000
Wire Wire Line
	4050 7100 4450 7100
Wire Wire Line
	2900 4700 2900 5400
Wire Wire Line
	3000 4800 3000 6200
Wire Wire Line
	10550 2750 10550 2200
Wire Wire Line
	12600 3250 12600 3450
Wire Wire Line
	1750 5400 1750 5800
Wire Wire Line
	1750 5800 1800 5800
Connection ~ 1750 5400
Wire Wire Line
	1750 6200 1750 6600
Wire Wire Line
	1750 6600 1800 6600
Connection ~ 1750 6200
Wire Wire Line
	1750 4600 1750 5000
Wire Wire Line
	1750 5000 1800 5000
Connection ~ 1750 4600
Connection ~ 3800 2100
Connection ~ 3800 2800
Connection ~ 6550 4600
Connection ~ 6550 4400
Connection ~ 6550 4000
Wire Wire Line
	12150 2100 12150 2400
Connection ~ 12150 2200
Wire Wire Line
	11450 2200 11450 2900
Wire Wire Line
	11650 2900 12150 2900
Connection ~ 12150 2900
Wire Wire Line
	13150 2750 13150 2700
Wire Wire Line
	12900 2700 12900 2750
Wire Wire Line
	12600 2750 12600 2700
Wire Wire Line
	11650 2800 11650 2900
Wire Wire Line
	11650 2200 11650 2400
Wire Wire Line
	9850 2300 9850 2200
Wire Wire Line
	9850 2750 9850 2700
Wire Wire Line
	9350 2300 9350 2200
Wire Wire Line
	9350 2700 9350 2750
Wire Wire Line
	12850 4250 12800 4250
Wire Wire Line
	12050 5050 12000 5050
Wire Wire Line
	11400 5650 11350 5650
Wire Wire Line
	11400 5450 11350 5450
Wire Wire Line
	11400 5850 11350 5850
Wire Wire Line
	10450 8150 10450 8250
Wire Wire Line
	10150 8200 10450 8200
Wire Wire Line
	10350 8200 10350 8150
Connection ~ 10450 8200
Wire Wire Line
	10250 8200 10250 8150
Connection ~ 10350 8200
Wire Wire Line
	10150 8200 10150 8150
Connection ~ 10250 8200
Wire Wire Line
	9300 7650 9300 7800
Wire Wire Line
	9300 7650 9350 7650
Wire Wire Line
	9350 7750 9300 7750
Connection ~ 9300 7750
Wire Wire Line
	9300 7150 9350 7150
Wire Wire Line
	9350 7250 9300 7250
Wire Wire Line
	5650 8100 5650 8250
Wire Wire Line
	5350 8150 5650 8150
Wire Wire Line
	5350 8150 5350 8100
Connection ~ 5650 8150
Wire Wire Line
	5450 8100 5450 8150
Connection ~ 5450 8150
Wire Wire Line
	5550 8100 5550 8150
Connection ~ 5550 8150
Wire Wire Line
	2300 6600 2350 6600
Wire Wire Line
	2350 5800 2300 5800
Wire Wire Line
	2350 5000 2300 5000
Wire Wire Line
	3400 3100 3450 3100
Wire Wire Line
	3300 2800 3350 2800
Wire Wire Line
	3800 2900 3850 2900
Wire Wire Line
	3800 1500 3800 1450
Wire Wire Line
	5550 2200 5550 2300
Connection ~ 5650 2200
Wire Wire Line
	5450 2200 5450 2300
Connection ~ 5550 2200
Wire Wire Line
	5350 2200 5350 2300
Connection ~ 5450 2200
Wire Wire Line
	6400 2200 6350 2200
Wire Wire Line
	12150 1450 12150 1500
$EndSCHEMATC
