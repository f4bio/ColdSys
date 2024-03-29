; TenSystem for Rainmeter - Disk IO Widget
; Common back-end code
; Version 4.1.3

[Rainmeter]
 ; This is used to show the Task Manager and Settings buttons only when the mouse is hovering over the widget.
 MouseOverAction=[!ShowMeter Settings][!ShowMeter LaunchTaskManager][!Redraw]
 MouseLeaveAction=[!HideMeter Settings][!HideMeter LaunchTaskManager][!Redraw]
 AccurateText=1

[Variables]
 ; This is where TenSystem's settings are stored.
 @include=#@#\Variables.inc

[MeasurePhysMemUsed]
Measure=PhysicalMemory
UpdateDivider=2

[mRAMUsage]
Measure=Plugin
Plugin=OpenHardwareMonitor.dll
Namespace=LibreHardwareMonitor
HardwareType=Memory
SensorType=Load

[MeasureRAM]
 ; This measures the amount of in-use memory.
Measure=PhysicalMemory

[CheckDiskStatus]
 ; This is used to check whether the disk is being monitored has changed its type.
 ; This can happen if the disk being monitored is a removable disk that has been removed,
 ; or more unlikely (but possibly) because the drive letters have been changed.
 Measure=FreeDiskSpace
 Drive=#DiskToMeasure#
 Type=1
 IfCondition=(CheckDiskStatus = 3) || (CheckDiskStatus = 4)
 IfTrueAction=[!HideMeter ErrorBox][!ShowMeter DiskGraph][!ShowMeter DiskGraphMax][!SetOption Tooltip MeterStyle "TooltipSingleDisk"][!UpdateMeter Tooltip][!Redraw]
 IfFalseAction=[!ShowMeter ErrorBox][!HideMeter DiskGraph][!HideMeter DiskGraphMax][!SetOption Tooltip MeterStyle "TooltipError"][!UpdateMeter Tooltip][!Redraw]

[MeasureDiskToMeasure]
 ; This is used to check whether the Disk IO widget is set to monitor all disks or just a single disk,
 ; and change/show/hide the tooltips and labels as appropriate.
 ; This overrides the disk status check above when DiskToMeasure is set to _Total,
 ; and ensures the widget displays normally,
 ; which would always return a value of 1 (Removed) otherwise.
 Measure=String
 String=#DiskToMeasure#
 IfMatch=_Total
 IfMatchAction=[!HideMeter ErrorBox][!ShowMeter DiskGraph][!ShowMeter DiskGraphMax][!HideMeter LabelDiskLetter][!SetOption Tooltip MeterStyle "TooltipAllDisks"][!UpdateMeter Tooltip][!Redraw]
 IfNotMatchAction=[!ShowMeter LabelDiskLetter][!Redraw]
 UpdateDivider=-1

; The following measures are used to measure the amount of disk I/O activity.
; Rainmeter does not have a built-in disk I/O measure, so we will have to use UsageMonitor for this purpose.
; Previously, the Disk IO widget used the PhysicalDisk object in PerfMon,
; but as PhysicalDisk appended each Instance's name with the disk's number,
; simply passing the letter of the Disk as the Instance name (referred to as Name by UsageMonitor) will cause things to fail.
; Therefore, we switch to using the LogicalDisk object instead,
; which simply uses the drive letters as each Instance's name.
; This is how TJ Markham's Win10 Widgets can select which disk to monitor from PerfMon simply by using the drive letter.

[MeasureDiskActivityPercent]
 ; This measures the percentage of time the disks were busy reading/writing data across all processes and disks.
 Measure=Plugin
 Plugin=UsageMonitor
 ; Assume an empty DiskToMeasure value to be the default (_Total).
 Substitute="":"_Total"
 Category=LogicalDisk
 Counter=% Disk Time
 Name=#DiskToMeasure#

[CalcDiskActivityPercentLimited]
 ; As explained in this Microsoft article (https://docs.microsoft.com/en-us/archive/blogs/askcore/windows-performance-monitor-disk-counters-explained),
 ; % Disk Time can exceed 100% at times.
 ; (The above mainly applies to the PhysicalDisk object, but this happens with the LogicalDisk object too.)
 ; To ensure that we don't show values over 100%,
 ; this measure limits values returned by % Disk Time to no more than 100%.
 ; This code is based off code by TJ Markham in his Win10 Widgets' Performance - Disk widgets,
 ; modified for use with values output by the UsageMonitor plugin rather than the PerfMon plugin.
 Measure=Calc
 Formula=[MeasureDiskActivityPercent:] < 100 ? [MeasureDiskActivityPercent:] : 100
 MaxValue=100
 MinValue=0
 DynamicVariables=1

[MeasureDiskReadSpeed]
 ; This measures the number of bytes the disks are reading per second.
 Measure=Plugin
 Plugin=UsageMonitor
 ; Assume an empty DiskToMeasure value to be the default (_Total).
 Substitute="":"_Total"
 Category=LogicalDisk
 Counter=Disk Read Bytes/sec
 Name=#DiskToMeasure#
 
[MeasureDiskWriteSpeed]
 ; This measures the number of bytes the disks are writing per second.
 Measure=Plugin
 Plugin=UsageMonitor
 ; Assume an empty DiskToMeasure value to be the default (_Total).
 Substitute="":"_Total"
 Category=LogicalDisk
 Counter=Disk Write Bytes/sec
 Name=#DiskToMeasure#
 
; We use a Calc measure here to retrieve the numerical value returned from MeasureDiskReadSpeed and MeasureDiskWriteSpeed,
; as the measure above returns both a string and numerical value,
; with the string value being the name of the instance in PerfMon instead of the numerical value.
; If we simply passed MeasureDiskReadSpeed and MeasureDiskWriteSpeed into the String meter for the tooltip below, 
; it will take the string value instead of the numerical value, 
; and cause it to display "_Total" instead.
[CalcDiskReadSpeed]
 Measure=Calc
 ; Be sure to add the colon at the end of the measure name as a section variable,
 ; to ensure that we are retrieving the numerical value.
 Formula=[MeasureDiskReadSpeed:]
 DynamicVariables=1
 
[CalcDiskWriteSpeed]
 Measure=Calc
 ; Be sure to add the colon at the end of the measure name as a section variable,
 ; to ensure that we are retrieving the numerical value.
 Formula=[MeasureDiskWriteSpeed:]
 DynamicVariables=1
 
[BG]
 ; This defines the size of the skin itself.
 ; The colour is defined in the variant skins themselves.
 Meter=Shape
 Shape=Rectangle 0,0,200,50 | StrokeWidth 0 | Stroke Color 0,0,0,0 | Extend BackgroundColor
 BackgroundColor=Fill Color #BGColor#

; These MeterStyles define the different tooltips to display depending on the state of the Disk IO Widget (measuring all disks, measuring a single disk, or error).
[TooltipAllDisks]
 TooltipTitle="Disk I/O (All Physical Disks)"
 TooltipText="Active time (average): [CalcDiskActivityPercentLimited:0]%#CRLF#Read speed (total): %1B/s#CRLF#Write speed (total): %2B/s"

[TooltipSingleDisk]
 TooltipTitle="Disk I/O (#DiskToMeasure#)"
 TooltipText="Active time: [CalcDiskActivityPercentLimited:0]%#CRLF#Read speed: %1B/s#CRLF#Write speed: %2B/s"

[TooltipError]
 TooltipTitle="Disk I/O (Error)"
 TooltipText="Open Settings for details and to reconfigure Disk I/O."

[Tooltip]
 ; The tooltip displays the following info:
 ; - the active disk time
 ; - the read speed
 ; - the write speed
 ; or an error message if the Disk IO widget is misconfigured,
 ; using the MeterStyles above.
 ; The text meter used here is essentially made invisible but not hidden,
 ; as that would cause the tooltip to not show.
 Meter=STRING
 MeasureName=CalcDiskReadSpeed
 MeasureName2=CalcDiskWriteSpeed
 X=0
 Y=0
 W=200
 H=50
 FontColor=0,0,0,0
 FontSize=0
 DynamicVariables=1
 Text=""
 ; The tooltip text is dynamically set by MeasureDiskToMeasure and CheckDiskStatus above by setting the appropriate MeterStyle.
 MeterStyle=TooltipAllDisks

[DiskGraphMax]
 Meter=Line
 MeasureName=CalcDiskActivityPercentLimited
 LineColor=#GraphLineColor#
 X=0
 Y=0
 W=200
 H=50
 AntiAlias=1
 GraphStart=Right
 DynamicVariables=1

[DiskGraph]
 Meter=Histogram
 MeasureName=CalcDiskActivityPercentLimited
 PrimaryColor=#GraphFillColor#
 X=0
 Y=0
 W=200
 H=50
 AntiAlias=1
 GraphStart=Right
 DynamicVariables=1

[Label]
 ; This is the background text on the widget.
 ; The font colour is set in the variant skins themselves.
 Meter=STRING
 X=190
 Y=7
 FontColor=#LabelColor#
 FontFace=Segoe UI Light
 FontSize=20
 StringAlign=Right
 StringStyle=NORMAL
 AntiAlias=1
 Text=""

[LabelRAMpos]
 ; This is the background text on the widget.
 ; The font colour is set in the variant skins themselves.
 Meter=STRING
 ;MeasureName=MeasurePhysMemUsed
 X=190
 Y=7
 FontColor=#LabelColor#
 FontFace=Segoe UI Light
 FontSize=20
 StringAlign=Right
 StringStyle=NORMAL
 AntiAlias=1
 AutoScale=1
 ;Text="%1B"

[LabelDISK]
 Meter=STRING
 X=5
 Y=13
 FontColor=#blue#,#atext#
 FontFace=Segoe UI Light
 FontSize=8
 StringAlign=Left
 StringStyle=NORMAL
 AntiAlias=0
 Text="DISK"

[LabelNET]
 Meter=STRING
 X=5
 Y=24
 FontColor=#yellow#,#atext#
 FontFace=Segoe UI Light
 FontSize=8
 StringAlign=Left
 StringStyle=NORMAL
 AntiAlias=0
 Text=""

[LabelDiskLetter]
 ; This shows the letter of the disk drive currently being monitored when set to monitor a single disk.
 ; If DiskToMeasure is set to _Total, the MeasureDiskToMeasure measure should hide this label automatically.
 Meter=STRING
 X=187
 Y=1
 FontColor=#LabelColor#
 FontFace=Segoe UI Light
 FontSize=25
 StringAlign=Right
 StringStyle=NORMAL
 AntiAlias=1
 Text=#DiskToMeasure#
 DynamicVariables=1

[ErrorBox]
 ; This is the error message overlay that shows if the Disk I/O widget has been configured to monitor the wrong type of disk.
 Meter=String
 Text="Error, check Settings."
 FontColor=#ErrorBoxTextColor#
 FontFace=Segoe UI
 FontSize=10
 SolidColor=#ErrorBoxColor#
 StringAlign=LeftCenter
 StringStyle=Normal
 Padding=13,0,-13,0
 AntiAlias=1
 X=0
 Y=25
 W=200
 H=50
 Hidden=1

; The graph consists of two parts: a Line meter and a Histogram meter.
; The Line meter is used to visually display the peaks of the graphs,
; while the Histogram meter is used to "fill in" the graph beneath the Line meter.
; The graph's colours are set in the variant skins themselves.

[ToolbarButtonBG]
 ; This is the background that appears when hovering over each of the toolbar's buttons.
 ; The position of the toolbar button background is dynamically set when hovering over each button.
 ; Dynamically repositioning a single shape meter on hover instead of having a separate hover button background for each button means less repetitive Meters need to be used,
 ; allowing one Shape meter to suffice for all the toolbar buttons instead of each requiring their own Meter.
 Meter=Shape
 Shape=Rectangle 0,0,24,24 | FillColor #ToolbarButtonHoverColor#,60 | StrokeWidth 1 | StrokeColor #ToolbarButtonHoverColor#,192
 Hidden=1

[Settings]
 ; This String meter displays a glyph from the Segoe MDL2 Assets font and acts as a link to launch the Settings skin for the user to configure the maximum scale of the graph.
 Meter=String
 ; Segoe MDL2 Assets character:
 ; Glyph name: Settings
 ; Glyph code: U+E713
 Text=[\xE713]
 FontColor=#ToolbarButtonGlyphColor#
 FontFace=Segoe MDL2 Assets 
 FontSize=12
 StringStyle=Normal
 Padding=4,4,-4,-4
 AntiAlias=1
 X=151
 Y=1
 W=24
 H=24
 Hidden=1
 TooltipText="Settings"
 ; Hide the informational tooltip when the mouse is hovering over this meter to ensure that only one tooltip is being displayed at any one time,
 ; as well as giving it a background colour to make it look like a button.
 MouseOverAction=[!SetOption ToolbarButtonBG X [#CURRENTSECTION#:X]][!SetOption ToolbarButtonBG Y [#CURRENTSECTION#:Y]][!UpdateMeter ToolbarButtonBG][!ShowMeter ToolbarButtonBG][!HideMeter Tooltip][!Redraw]
 MouseLeaveAction=[!HideMeter ToolbarButtonBG][!ShowMeter Tooltip][!Redraw]
 LeftMouseUpAction=[!ActivateConfig "#CURRENTCONFIG#\Settings" "Settings.ini"]

[LaunchTaskManager]
 ; This String meter displays a glyph from the Segoe MDL2 Assets font and acts as a link to launch Task Manager.
 Meter=String
 ; Segoe MDL2 Assets character:
 ; Glyph name: Diagnostic
 ; Glyph code: U+E9D9
 Text=[\xE9D9]
 FontColor=#ToolbarButtonGlyphColor#
 FontFace=Segoe MDL2 Assets 
 FontSize=12
 StringStyle=Normal
 Padding=4,4,-4,-4
 AntiAlias=1
 X=175
 Y=1
 W=24
 H=24
 Hidden=1
 TooltipText="Open Task Manager"
 ; Hide the informational tooltip when the mouse is hovering over this meter to ensure that only one tooltip is being displayed at any one time,
 ; as well as giving it a background colour to make it look like a button.
 MouseOverAction=[!SetOption ToolbarButtonBG X [#CURRENTSECTION#:X]][!SetOption ToolbarButtonBG Y [#CURRENTSECTION#:Y]][!UpdateMeter ToolbarButtonBG][!ShowMeter ToolbarButtonBG][!HideMeter Tooltip][!Redraw]
 MouseLeaveAction=[!HideMeter ToolbarButtonBG][!ShowMeter Tooltip][!Redraw]
 LeftMouseUpAction=["%windir%\system32\taskmgr.exe"]