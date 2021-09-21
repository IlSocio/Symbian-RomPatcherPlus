

ROMPATCHER+ v2.6
http://www.symbian-toys.com/RomPatcherPlus.aspx


RomPatcher+ is an improved and fixed version of the RomPatcher software developed by ZoRn.
Using RomPatcher+ it is possibile to reach a new level of customization for you Symbian phone.

This is the list of the main differences in comparison with RomPatcher by ZoRn:
- New: support for compiled patches
- New: can dump the whole rom content to file \romdumpplus.dmp
- New: can dump the SuperPage content to file \superpage.dmp
- New: full support for DomainSrv.exe autostart 
- New: introduced new commands ord_rel, ord_snr which allow to patch DLL using ordinal
- New: can be integrated in cooked ROM firmware
- New: it is now possible to apply/remove patches with just 1 click.
- New: added vertical scrollbar.
- New: introduced the new command "+SuperPage" which allow to patch the RAM area where is stored the SuperPage.
- New: introduced support for a new commands, info: return: error: check the demo1-4.rmp patches included.
- New: introduced #ifdef, #ifndef, #else, #endif, #define statements. This will allow to retrieve system values at runtime to create more flexible and powerful patches. Check the ReadMe_Macros.txt for all the details.
- New: you can search / filter the patches in the list. Just start typing the patch name to apply the filter.
- New: wildcard ?? support for patches.
- New: RP+ can be included in cooked ROM firmware
- Fixed: Kern-Exec0 error when closing RomPatcher after a patch was applied.
- Fixed: doesn't unexpectedly crash when using large and complex patches.
- Fixed: RP+ can correcly patch contiguous shadow ram pages without any crash.
- Improved: better compatibility (supports also the OmniaHD, 5630, E52 and probably future devices).
- Improved: the "Patch Info..." option shows all the information lines contained in the patch.



*** Patcher.ldd Not Found Error *** 
If you launch RP+ immediately after it has been installed you could get the "patcher.ldd not found" error.
Just reboot the device and launch again RP+



*** Change-Log v2.5 > v2.6 *** 
+ New: Support for compiled patches (.RPP files) Patch-Makers can now distribuite compiled patches to protect their work. Use the "For Patch-Makers\CompileRMP.exe" to compile your .RMP patches in .RPP
+ Improved: Added DomainSrv.exe by wadowice. Must be installed manually, you can find it inside the "For Advanced Users" folder



*** Other Important Stuff ***
Feel free to post RP+ v2.6 on forums or everywhere you like, I want just kindly ask you to include the warning message below which is important for the survivor of the project:

[quote]
Warning: If you like this software then support RP+ with a donation.
http://www.symbian-toys.com/rompatcherplus.aspx
[/quote]


Regards,
Marco.


Thanks to:
- ZoRn for the original RomPatcher idea.
- FCA00000 for its brilliant brain.
- wadowice, templove, bugb, abgnokia, leftup, CODeRUS, Leonapapa, PNHT and others, for their contribution to the underground Symbian scene, keep up with the good job!
- megaexer for svg icon.
