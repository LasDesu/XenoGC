EESchema Schematic File Version 4
EELAYER 30 0
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
L mylib:ATtiny85 DD1
U 1 1 5D306471
P 4900 3400
F 0 "DD1" H 5550 3550 60  0000 C CNN
F 1 "ATtiny85" H 5550 3050 50  0000 C CNN
F 2 "Housings_SOIC:SOIJ-8_5.3x5.3mm_Pitch1.27mm" H 5200 3050 60  0001 C CNN
F 3 "" H 5200 3050 60  0000 C CNN
	1    4900 3400
	1    0    0    -1  
$EndComp
$Comp
L mylib:PIN P5
U 1 1 5D307613
P 4400 2900
F 0 "P5" H 4480 2900 40  0001 L CNN
F 1 "2-CLK" H 4400 2975 40  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 4400 2900 60  0001 C CNN
F 3 "" H 4400 2900 60  0000 C CNN
	1    4400 2900
	0    -1   -1   0   
$EndComp
$Comp
L mylib:PIN P6
U 1 1 5D30788D
P 6600 2900
F 0 "P6" H 6680 2900 40  0001 L CNN
F 1 "6-STR" H 6600 2975 40  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 6600 2900 60  0001 C CNN
F 3 "" H 6600 2900 60  0000 C CNN
	1    6600 2900
	0    -1   -1   0   
$EndComp
$Comp
L mylib:PIN P1
U 1 1 5D307962
P 5300 2700
F 0 "P1" H 5380 2700 40  0001 L CNN
F 1 "10-VCC" H 5300 2775 40  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 5300 2700 60  0001 C CNN
F 3 "" H 5300 2700 60  0000 C CNN
	1    5300 2700
	-1   0    0    1   
$EndComp
$Comp
L mylib:VCC #PWR0101
U 1 1 5D307AEA
P 5600 2700
F 0 "#PWR0101" H 5600 2700 30  0001 C CNN
F 1 "VCC" H 5600 2800 40  0000 C CNN
F 2 "" H 5600 2700 60  0000 C CNN
F 3 "" H 5600 2700 60  0000 C CNN
	1    5600 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	5600 2700 5500 2700
$Comp
L mylib:PIN P2
U 1 1 5D307DC9
P 5300 3000
F 0 "P2" H 5380 3000 40  0001 L CNN
F 1 "1-GND" H 5300 3075 40  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 5300 3000 60  0001 C CNN
F 3 "" H 5300 3000 60  0000 C CNN
	1    5300 3000
	-1   0    0    1   
$EndComp
$Comp
L mylib:GND #PWR0102
U 1 1 5D30800D
P 5500 3100
F 0 "#PWR0102" H 5500 3100 30  0001 C CNN
F 1 "GND" H 5500 3030 30  0001 C CNN
F 2 "" H 5500 3100 60  0000 C CNN
F 3 "" H 5500 3100 60  0000 C CNN
	1    5500 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3100 5500 3000
Wire Wire Line
	5500 3000 5400 3000
$Comp
L mylib:CAP C1
U 1 1 5D308335
P 5500 2900
F 0 "C1" H 5500 3025 50  0000 C CNN
F 1 "CAP" H 5500 2775 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5500 2850 60  0001 C CNN
F 3 "" H 5500 2850 60  0000 C CNN
	1    5500 2900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5500 2800 5500 2700
Connection ~ 5500 2700
Wire Wire Line
	5500 2700 5400 2700
Connection ~ 5500 3000
$Comp
L mylib:VCC #PWR0103
U 1 1 5D309E6B
P 6000 4000
F 0 "#PWR0103" H 6000 4000 30  0001 C CNN
F 1 "VCC" H 6000 4100 40  0000 C CNN
F 2 "" H 6000 4000 60  0000 C CNN
F 3 "" H 6000 4000 60  0000 C CNN
	1    6000 4000
	1    0    0    -1  
$EndComp
$Comp
L mylib:LED VD1
U 1 1 5D30A158
P 5300 4200
F 0 "VD1" H 5300 4400 50  0000 C CNN
F 1 "LED" H 5300 4100 50  0000 C CNN
F 2 "LEDs:LED_0805_HandSoldering" H 5300 4150 60  0001 C CNN
F 3 "" H 5300 4150 60  0000 C CNN
	1    5300 4200
	-1   0    0    -1  
$EndComp
$Comp
L mylib:LED VD2
U 1 1 5D30A88B
P 5300 4600
F 0 "VD2" H 5300 4800 50  0000 C CNN
F 1 "LED" H 5300 4500 50  0000 C CNN
F 2 "LEDs:LED_0805_HandSoldering" H 5300 4550 60  0001 C CNN
F 3 "" H 5300 4550 60  0000 C CNN
	1    5300 4600
	-1   0    0    -1  
$EndComp
$Comp
L mylib:RES R1
U 1 1 5D30B040
P 5700 4200
F 0 "R1" H 5700 4300 50  0000 C CNN
F 1 "330" H 5700 4100 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5700 4150 60  0001 C CNN
F 3 "" H 5700 4150 60  0000 C CNN
	1    5700 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 4200 5400 4200
$Comp
L mylib:RES R2
U 1 1 5D30B2CE
P 5700 4600
F 0 "R2" H 5700 4700 50  0000 C CNN
F 1 "330" H 5700 4500 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5700 4550 60  0001 C CNN
F 3 "" H 5700 4550 60  0000 C CNN
	1    5700 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 4600 5500 4600
Wire Wire Line
	5900 4600 6000 4600
Wire Wire Line
	6000 4600 6000 4200
Wire Wire Line
	5900 4200 6000 4200
Connection ~ 6000 4200
Wire Wire Line
	6000 4200 6000 4000
Wire Wire Line
	4400 3600 4400 3000
Wire Wire Line
	4700 4200 5200 4200
Wire Wire Line
	6600 3600 6200 3600
Wire Wire Line
	6600 3000 6600 3600
Wire Wire Line
	6800 3500 6800 3000
Wire Wire Line
	6200 3500 6800 3500
Wire Wire Line
	6300 3400 6200 3400
Wire Wire Line
	6300 3000 6300 3400
$Comp
L mylib:PIN P4
U 1 1 5D3074C4
P 6800 2900
F 0 "P4" H 6880 2900 40  0001 L CNN
F 1 "4-OUT" H 6800 2975 40  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 6800 2900 60  0001 C CNN
F 3 "" H 6800 2900 60  0000 C CNN
	1    6800 2900
	0    1    -1   0   
$EndComp
$Comp
L mylib:PIN P3
U 1 1 5D3070F9
P 6300 2900
F 0 "P3" H 6380 2900 40  0001 L CNN
F 1 "3-IN" H 6300 2975 40  0000 C CNN
F 2 "Measurement_Points:Measurement_Point_Square-SMD-Pad_Small" H 6300 2900 60  0001 C CNN
F 3 "" H 6300 2900 60  0000 C CNN
	1    6300 2900
	0    1    -1   0   
$EndComp
Wire Wire Line
	4700 3500 4900 3500
Wire Wire Line
	4700 3500 4700 4200
Wire Wire Line
	4400 3600 4900 3600
Wire Wire Line
	4600 4600 4600 3400
Wire Wire Line
	4600 3400 4900 3400
Wire Wire Line
	4600 4600 5200 4600
$EndSCHEMATC
