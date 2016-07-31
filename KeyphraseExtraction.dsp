# Microsoft Developer Studio Project File - Name="KeyphraseExtraction" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=KeyphraseExtraction - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "KeyphraseExtraction.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "KeyphraseExtraction.mak" CFG="KeyphraseExtraction - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KeyphraseExtraction - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "KeyphraseExtraction - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "KeyphraseExtraction - Win32 Fast" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "KeyphraseExtraction - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /stack:0x4c4b40 /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:yes /debug /machine:I386

!ELSEIF  "$(CFG)" == "KeyphraseExtraction - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /stack:0x2faf080 /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "KeyphraseExtraction - Win32 Fast"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "KeyphraseExtraction___Win32_Fast"
# PROP BASE Intermediate_Dir "KeyphraseExtraction___Win32_Fast"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Fast"
# PROP Intermediate_Dir "Fast"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /stack:0x3d0900 /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:yes /debug /machine:I386
# ADD LINK32 /nologo /version:1.1 /stack:0x3d0900 /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386
# SUBTRACT LINK32 /incremental:yes /debug

!ENDIF 

# Begin Target

# Name "KeyphraseExtraction - Win32 Release"
# Name "KeyphraseExtraction - Win32 Debug"
# Name "KeyphraseExtraction - Win32 Fast"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Data.cpp
# End Source File
# Begin Source File

SOURCE=.\Extraction.cpp
# End Source File
# Begin Source File

SOURCE=.\HashTable.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyphraseExtraction.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyphraseExtraction.rc
# End Source File
# Begin Source File

SOURCE=.\KeyphraseExtractionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SortHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SortListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Unit.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Data.h
# End Source File
# Begin Source File

SOURCE=.\Extraction.h
# End Source File
# Begin Source File

SOURCE=.\HashTable.h
# End Source File
# Begin Source File

SOURCE=.\ICTCLAS2011.h
# End Source File
# Begin Source File

SOURCE=.\KeyphraseExtraction.h
# End Source File
# Begin Source File

SOURCE=.\KeyphraseExtractionDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SortHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SortListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Unit.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\20120427024017227_easyicon_cn_48.ico
# End Source File
# Begin Source File

SOURCE=.\app.ico
# End Source File
# Begin Source File

SOURCE=.\res\extract.ico
# End Source File
# Begin Source File

SOURCE=.\res\KeyphraseExtraction.ico
# End Source File
# Begin Source File

SOURCE=.\res\KeyphraseExtraction.rc2
# End Source File
# End Group
# Begin Group "mschart"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mschart\font.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\font.h
# End Source File
# Begin Source File

SOURCE=.\mschart\mschart.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\mschart.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcaxis.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcaxis.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcaxisgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcaxisgrid.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcaxisscale.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcaxisscale.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcaxistitle.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcaxistitle.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcbackdrop.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcbackdrop.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcbrush.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcbrush.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vccategoryscale.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vccategoryscale.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vccolor.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vccolor.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vccoor.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vccoor.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcdatagrid.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcdatagrid.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcdatapoint.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcdatapoint.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcdatapointlabel.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcdatapointlabel.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcdatapoints.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcdatapoints.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcfill.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcfill.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcfont.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcfont.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcfootnote.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcfootnote.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcframe.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcframe.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcintersection.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcintersection.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vclabel.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vclabel.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vclabels.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vclabels.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vclcoor.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vclcoor.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vclegend.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vclegend.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vclight.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vclight.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vclightsource.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vclightsource.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vclightsources.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vclightsources.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vclocation.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vclocation.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcmarker.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcmarker.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcpen.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcpen.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcplot.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcplot.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcplotbase.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcplotbase.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcrect.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcrect.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcseries.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcseries.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcseriescollection.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcseriescollection.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcseriesmarker.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcseriesmarker.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcseriesposition.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcseriesposition.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcshadow.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcshadow.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcstatline.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcstatline.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vctextlayout.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vctextlayout.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vctick.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vctick.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vctitle.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vctitle.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcvaluescale.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcvaluescale.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcview3d.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcview3d.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcwall.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcwall.h
# End Source File
# Begin Source File

SOURCE=.\mschart\vcweighting.cpp
# End Source File
# Begin Source File

SOURCE=.\mschart\vcweighting.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section KeyphraseExtraction : {6237536E-A17D-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcAxisTitle
# 	2:10:HeaderFile:mschart\vcaxistitle.h
# 	2:8:ImplFile:mschart\vcaxistitle.cpp
# End Section
# Section KeyphraseExtraction : {AFE57020-7409-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcBrush
# 	2:10:HeaderFile:mschart\vcbrush.h
# 	2:8:ImplFile:mschart\vcbrush.cpp
# End Section
# Section KeyphraseExtraction : {6CB603A0-8F70-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcDataGrid
# 	2:10:HeaderFile:mschart\vcdatagrid.h
# 	2:8:ImplFile:mschart\vcdatagrid.cpp
# End Section
# Section KeyphraseExtraction : {21645F62-90F0-11CE-86B3-444553540000}
# 	2:5:Class:CVcView3d
# 	2:10:HeaderFile:mschart\vcview3d.h
# 	2:8:ImplFile:mschart\vcview3d.cpp
# End Section
# Section KeyphraseExtraction : {95C52B60-83B1-11CE-BECC-00AA0042CB33}
# 	2:5:Class:CVcLegend
# 	2:10:HeaderFile:mschart\vclegend.h
# 	2:8:ImplFile:mschart\vclegend.cpp
# End Section
# Section KeyphraseExtraction : {3080E742-813F-11CE-BECC-00AA0042CB33}
# 	2:5:Class:CVcMarker
# 	2:10:HeaderFile:mschart\vcmarker.h
# 	2:8:ImplFile:mschart\vcmarker.cpp
# End Section
# Section KeyphraseExtraction : {51DCC622-95B7-11CE-86B3-444553540000}
# 	2:5:Class:CVcSeriesPosition
# 	2:10:HeaderFile:mschart\vcseriesposition.h
# 	2:8:ImplFile:mschart\vcseriesposition.cpp
# End Section
# Section KeyphraseExtraction : {EBF97E20-731F-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcCoor
# 	2:10:HeaderFile:mschart\vccoor.h
# 	2:8:ImplFile:mschart\vccoor.cpp
# End Section
# Section KeyphraseExtraction : {FD30FB02-789C-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcFill
# 	2:10:HeaderFile:mschart\vcfill.h
# 	2:8:ImplFile:mschart\vcfill.cpp
# End Section
# Section KeyphraseExtraction : {2E6A37A2-77FC-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcBackdrop
# 	2:10:HeaderFile:mschart\vcbackdrop.h
# 	2:8:ImplFile:mschart\vcbackdrop.cpp
# End Section
# Section KeyphraseExtraction : {62375366-A17D-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcIntersection
# 	2:10:HeaderFile:mschart\vcintersection.h
# 	2:8:ImplFile:mschart\vcintersection.cpp
# End Section
# Section KeyphraseExtraction : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:mschart\font.h
# 	2:8:ImplFile:mschart\font.cpp
# End Section
# Section KeyphraseExtraction : {6FE35CC0-CE50-11CE-8410-00AA0042CB33}
# 	2:5:Class:CVcLightSources
# 	2:10:HeaderFile:mschart\vclightsources.h
# 	2:8:ImplFile:mschart\vclightsources.cpp
# End Section
# Section KeyphraseExtraction : {CD9EFA60-AA40-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcLightSource
# 	2:10:HeaderFile:mschart\vclightsource.h
# 	2:8:ImplFile:mschart\vclightsource.cpp
# End Section
# Section KeyphraseExtraction : {C20E5260-B06F-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcDataPoints
# 	2:10:HeaderFile:mschart\vcdatapoints.h
# 	2:8:ImplFile:mschart\vcdatapoints.cpp
# End Section
# Section KeyphraseExtraction : {027D7900-A023-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcAxis
# 	2:10:HeaderFile:mschart\vcaxis.h
# 	2:8:ImplFile:mschart\vcaxis.cpp
# End Section
# Section KeyphraseExtraction : {2F6DD6A0-95E5-11CE-86B3-444553540000}
# 	2:5:Class:CVcStatLine
# 	2:10:HeaderFile:mschart\vcstatline.h
# 	2:8:ImplFile:mschart\vcstatline.cpp
# End Section
# Section KeyphraseExtraction : {508D02E6-90FC-11CE-86B3-444553540000}
# 	2:5:Class:CVcWeighting
# 	2:10:HeaderFile:mschart\vcweighting.h
# 	2:8:ImplFile:mschart\vcweighting.cpp
# End Section
# Section KeyphraseExtraction : {FE5517C0-73FE-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcRect
# 	2:10:HeaderFile:mschart\vcrect.h
# 	2:8:ImplFile:mschart\vcrect.cpp
# End Section
# Section KeyphraseExtraction : {62375360-A17D-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcCategoryScale
# 	2:10:HeaderFile:mschart\vccategoryscale.h
# 	2:8:ImplFile:mschart\vccategoryscale.cpp
# End Section
# Section KeyphraseExtraction : {62375370-A17D-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcLabel
# 	2:10:HeaderFile:mschart\vclabel.h
# 	2:8:ImplFile:mschart\vclabel.cpp
# End Section
# Section KeyphraseExtraction : {6237536C-A17D-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcValueScale
# 	2:10:HeaderFile:mschart\vcvaluescale.h
# 	2:8:ImplFile:mschart\vcvaluescale.cpp
# End Section
# Section KeyphraseExtraction : {B54A6420-A25E-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcLight
# 	2:10:HeaderFile:mschart\vclight.h
# 	2:8:ImplFile:mschart\vclight.cpp
# End Section
# Section KeyphraseExtraction : {2F6DD6A4-95E5-11CE-86B3-444553540000}
# 	2:5:Class:CVcDataPoint
# 	2:10:HeaderFile:mschart\vcdatapoint.h
# 	2:8:ImplFile:mschart\vcdatapoint.cpp
# End Section
# Section KeyphraseExtraction : {C14E8B60-AE2B-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcSeriesCollection
# 	2:10:HeaderFile:mschart\vcseriescollection.h
# 	2:8:ImplFile:mschart\vcseriescollection.cpp
# End Section
# Section KeyphraseExtraction : {62B9A400-B93D-11CE-8410-00AA0042CB33}
# 	2:5:Class:CVcLabels
# 	2:10:HeaderFile:mschart\vclabels.h
# 	2:8:ImplFile:mschart\vclabels.cpp
# End Section
# Section KeyphraseExtraction : {508D02E0-90FC-11CE-86B3-444553540000}
# 	2:5:Class:CVcPlotBase
# 	2:10:HeaderFile:mschart\vcplotbase.h
# 	2:8:ImplFile:mschart\vcplotbase.cpp
# End Section
# Section KeyphraseExtraction : {E26C7700-756C-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcTextLayout
# 	2:10:HeaderFile:mschart\vctextlayout.h
# 	2:8:ImplFile:mschart\vctextlayout.cpp
# End Section
# Section KeyphraseExtraction : {62375364-A17D-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcAxisGrid
# 	2:10:HeaderFile:mschart\vcaxisgrid.h
# 	2:8:ImplFile:mschart\vcaxisgrid.cpp
# End Section
# Section KeyphraseExtraction : {2E6A37A0-77FC-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcFont
# 	2:10:HeaderFile:mschart\vcfont.h
# 	2:8:ImplFile:mschart\vcfont.cpp
# End Section
# Section KeyphraseExtraction : {31291E80-728C-11CF-93D5-0020AF99504A}
# 	2:21:DefaultSinkHeaderFile:mschart\mschart.h
# 	2:16:DefaultSinkClass:CMSChart
# End Section
# Section KeyphraseExtraction : {279B5A40-8098-11CE-BECC-00AA0042CB33}
# 	2:5:Class:CVcPen
# 	2:10:HeaderFile:mschart\vcpen.h
# 	2:8:ImplFile:mschart\vcpen.cpp
# End Section
# Section KeyphraseExtraction : {C82141A0-7571-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcTitle
# 	2:10:HeaderFile:mschart\vctitle.h
# 	2:8:ImplFile:mschart\vctitle.cpp
# End Section
# Section KeyphraseExtraction : {AFE57022-7409-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcShadow
# 	2:10:HeaderFile:mschart\vcshadow.h
# 	2:8:ImplFile:mschart\vcshadow.cpp
# End Section
# Section KeyphraseExtraction : {62375368-A17D-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcTick
# 	2:10:HeaderFile:mschart\vctick.h
# 	2:8:ImplFile:mschart\vctick.cpp
# End Section
# Section KeyphraseExtraction : {F75BA680-9057-115E-85B3-448553548000}
# 	2:5:Class:CVcPlot
# 	2:10:HeaderFile:mschart\vcplot.h
# 	2:8:ImplFile:mschart\vcplot.cpp
# End Section
# Section KeyphraseExtraction : {51DCC624-95B7-11CE-86B3-444553540000}
# 	2:5:Class:CVcSeriesMarker
# 	2:10:HeaderFile:mschart\vcseriesmarker.h
# 	2:8:ImplFile:mschart\vcseriesmarker.cpp
# End Section
# Section KeyphraseExtraction : {EDDF9242-764B-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcLocation
# 	2:10:HeaderFile:mschart\vclocation.h
# 	2:8:ImplFile:mschart\vclocation.cpp
# End Section
# Section KeyphraseExtraction : {EBF97E22-731F-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcLCoor
# 	2:10:HeaderFile:mschart\vclcoor.h
# 	2:8:ImplFile:mschart\vclcoor.cpp
# End Section
# Section KeyphraseExtraction : {0AA0FE20-912A-11CE-86B3-444553540000}
# 	2:5:Class:CVcSeries
# 	2:10:HeaderFile:mschart\vcseries.h
# 	2:8:ImplFile:mschart\vcseries.cpp
# End Section
# Section KeyphraseExtraction : {2E6A37A4-77FC-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcFrame
# 	2:10:HeaderFile:mschart\vcframe.h
# 	2:8:ImplFile:mschart\vcframe.cpp
# End Section
# Section KeyphraseExtraction : {6237536A-A17D-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcAxisScale
# 	2:10:HeaderFile:mschart\vcaxisscale.h
# 	2:8:ImplFile:mschart\vcaxisscale.cpp
# End Section
# Section KeyphraseExtraction : {00D6D1A0-728C-11CF-93D5-0020AF99504A}
# 	2:5:Class:CMSChart
# 	2:10:HeaderFile:mschart.h
# 	2:8:ImplFile:mschart.cpp
# End Section
# Section KeyphraseExtraction : {9BA79C60-7403-11CE-840F-00AA0042CB33}
# 	2:5:Class:CVcColor
# 	2:10:HeaderFile:mschart\vccolor.h
# 	2:8:ImplFile:mschart\vccolor.cpp
# End Section
# Section KeyphraseExtraction : {2F6DD6A2-95E5-11CE-86B3-444553540000}
# 	2:5:Class:CVcDataPointLabel
# 	2:10:HeaderFile:mschart\vcdatapointlabel.h
# 	2:8:ImplFile:mschart\vcdatapointlabel.cpp
# End Section
# Section KeyphraseExtraction : {508D02E8-90FC-11CE-86B3-444553540000}
# 	2:5:Class:CVcWall
# 	2:10:HeaderFile:mschart\vcwall.h
# 	2:8:ImplFile:mschart\vcwall.cpp
# End Section
# Section KeyphraseExtraction : {4F053F00-8396-11CE-BECC-00AA0042CB33}
# 	2:5:Class:CVcFootnote
# 	2:10:HeaderFile:mschart\vcfootnote.h
# 	2:8:ImplFile:mschart\vcfootnote.cpp
# End Section
