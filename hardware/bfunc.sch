EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 8050 2750 1350 700 
U 5E344A1B
F0 "DDS Chip" 50
F1 "dds_ic.sch" 50
F2 "DDS_FSYNC" I L 8050 3000 50 
F3 "DDS_SDATA" I L 8050 3100 50 
F4 "DDS_SCLK" I L 8050 3200 50 
$EndSheet
$Sheet
S 5000 2750 1350 700 
U 5E381CE2
F0 "MCU" 50
F1 "MCU.sch" 50
F2 "USB_D_N" B L 5000 3000 50 
F3 "USB_D_P" B L 5000 3100 50 
F4 "SPI1_SS_L" O R 6350 3000 50 
F5 "SPI1_SCK" O R 6350 3200 50 
F6 "SPI1_MISO" I R 6350 3300 50 
F7 "SPI1_MOSI" O R 6350 3100 50 
$EndSheet
Wire Wire Line
	6350 3100 8050 3100
Wire Wire Line
	8050 3000 6350 3000
Wire Wire Line
	6350 3200 8050 3200
Wire Wire Line
	6350 3300 6650 3300
NoConn ~ 6650 3300
Text Notes 6650 3550 0    50   ~ 0
NOTE: SPI1_MISO not used\n(unidirectional comms)
$Sheet
S 2050 2750 1400 850 
U 5E4A0C9F
F0 "USB/Power" 50
F1 "USB.sch" 50
F2 "USB_D_P" B R 3450 3100 50 
F3 "USB_D_N" B R 3450 3000 50 
$EndSheet
Wire Wire Line
	3450 3000 5000 3000
Wire Wire Line
	5000 3100 3450 3100
$Comp
L Connector:Conn_01x20_Male J9
U 1 1 5E3BFE58
P 5750 4400
F 0 "J9" V 5585 4328 50  0000 C CNN
F 1 "Conn_01x20_Male" V 5676 4328 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x20_P2.54mm_Vertical" H 5750 4400 50  0001 C CNN
F 3 "~" H 5750 4400 50  0001 C CNN
	1    5750 4400
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x20_Male J10
U 1 1 5E3C4F28
P 5750 5300
F 0 "J10" V 5585 5228 50  0000 C CNN
F 1 "Conn_01x20_Male" V 5676 5228 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x20_P2.54mm_Vertical" H 5750 5300 50  0001 C CNN
F 3 "~" H 5750 5300 50  0001 C CNN
	1    5750 5300
	0    1    1    0   
$EndComp
$EndSCHEMATC
