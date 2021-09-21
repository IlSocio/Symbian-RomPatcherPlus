
ROMPATCHER+ v2.6
http://www.symbian-toys.com/RomPatcherPlus.aspx


AVAILABLE MACROS:
The macros below can be used to check some system values at runtime...

**************************************************************************************************************

MANUFACTURER_(hex value)
See this link for values: http://www.forum.nokia.com/infocenter/index.jsp?topic=/S60_5th_Edition_Cpp_Developers_Library/GUID-35228542-8C95-4849-A73F-2B4F082F0C44/sdk/doc_source/faqSDK/faq_1333.html

; rmp example
#ifdef MANUFACTURER_2
; Nokia Manufacturer
; Abs....
#endif

**************************************************************************************************************

MACHINE_(hex value)
See this link for values: http://wiki.forum.nokia.com/index.php/S60_Platform_and_device_identification_codes

; rmp example
#ifdef MACHINE_2000DA56
; RP+ is running on a 5800 Device
; Abs....
#endif

**************************************************************************************************************

CPU_(dec value)
	enum TCPU
	{
	ECPU_ARM,
	ECPU_MCORE,
	ECPU_X86,
	};

; rmp example
#ifdef CPU_0
; Running on an ARM CPU
; Abs....
#endif

**************************************************************************************************************

CPUABI_(dec value)
	enum TCPUABI
	{
	ECPUABI_ARM4,
	ECPUABI_ARMI,
	ECPUABI_THUMB,
	ECPUABI_MCORE,
	ECPUABI_MSVC,
	ECPUABI_ARM5T,
	ECPUABI_X86,
	};

; rmp example
#ifdef CPUABI_2
; Running on an THUMB CPU
; Abs....
#endif

**************************************************************************************************************

PLATFORM_(dec value)
3.0 => 3rd Edition
3.1 => 3rd Edition FP1
3.2 => 3rd Edition FP2
5.0 => 5th Edition

; rmp example
#ifdef PLATFORM_3.2
; Running on an 3rd FP2 device
; Abs....
#endif

**************************************************************************************************************

LANG_(dec value)
See TLanguage for values: http://wiki.forum.nokia.com/index.php/TLanguage_enumeration

; rmp example
#ifdef LANG_5
; Phone Language is Italian
; Abs....
#endif

**************************************************************************************************************

FW_(dec value)

; rmp example
#ifdef FW_30.0.0.15
; Running on device with Firmware version 30.0.0.15
; Abs....
#endif

**************************************************************************************************************

RP+VERSION_(dec value) (currently useless, but could be useful in the future to keep backward compability)

; rmp example
#ifdef RP+VERSION_2.1
; Running on RP+ version 2.1
; Abs....
#endif

**************************************************************************************************************

Some useful tip&tricks when writing patches with #ifdef stuff...


How to manage NOT expressions using #ifndef...
if (!N95)
 doSomething()


; rmp example
#ifndef MACHINE_2000060B
 ;doSomething()
#endif


**************************************************************************************************************

How to manage OR expressions using #ifdef...

if (N95 || N96 || 5800)
 doSomething()


; rmp example
#ifdef MACHINE_2000060B
 #define mycheck
#endif
#ifdef MACHINE_20002D82
 #define mycheck
#endif
#ifdef MACHINE_2000DA56
 #define mycheck
#endif

#ifdef mycheck
 ;doSomething()
#endif


**************************************************************************************************************

How to manage AND expressions using #ifdef
if (N95 && FW == 30.0.015)
 doSomething();


; rmp example
#ifdef MACHINE_2000060B
	#ifdef FW_30.0.015
		; doSomething()
	#enif
#endif

**************************************************************************************************************

