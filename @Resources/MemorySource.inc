; TenSystem for Rainmeter - Memory Widget
; Common back-end code
; Version 4.1.3

[Rainmeter]
 ; This is used to show the Task Manager button only when the mouse is hovering over the widget.
 MouseOverAction=[!ShowMeter LaunchTaskManager][!Redraw]
 MouseLeaveAction=[!HideMeter LaunchTaskManager][!Redraw]
 AccurateText=1

[mRAMUsage]
Measure=Plugin
Plugin=OpenHardwareMonitor.dll
Namespace=LibreHardwareMonitor
HardwareType=Memory
SensorType=Load

[MeasurePhysMemTotal]
Measure=PhysicalMemory
Total=1

[MeasurePhysMemUsed]
Measure=PhysicalMemory
UpdateDivider=2

[MeasureUsedPercent]
Measure=Calc
Formula=(MeasurePhysMemUsed / MeasurePhysMemTotal) * 100

[MeasureRAM]
 ; This measures the amount of in-use memory.
 Measure=PhysicalMemory
 
[MeasureRAMFree]
 ; This measures the amount of free memory using the same measure as above,
 ; but with InvertMeasure set to 1.
 Measure=PhysicalMemory
 InvertMeasure=1
 
[MeasureRAMTotal]
 ; This measures the total amount of memory installed by setting Total to 1.
 Measure=PhysicalMemory
 Total=1
 
; These Calc measures convert the amount of memory from bytes into gigabytes.
; To get gigabytes, we divide the number of bytes by 1073741824.
[CalcRAM]
 Measure=Calc
 Formula=[MeasureRAM:]/1073741824
 DynamicVariables=1
 
[CalcRAMFree]
 Measure=Calc
 Formula=[MeasureRAMFree:]/1073741824
 DynamicVariables=1

[CalcRAMTotal]
 Measure=Calc
 Formula=[MeasureRAMTotal:]/1073741824
 DynamicVariables=1
 
[BG]
 ; This defines the size of the skin itself.
 ; The colour is defined in the variant skins themselves.
 Meter=Shape
 Shape=Rectangle 0,0,200,50 | StrokeWidth 0 | Stroke Color 0,0,0,0 | Extend BackgroundColor
 BackgroundColor=Fill Color #BGColor#
 
[Tooltip]
 ; The tooltip displays the following info:
 ; - memory in use (both in gigabytes and as a percentage)
 ; - memory available
 ; - total memory installed on the system
 ; The text meter used here is essentially made invisible but not hidden,
 ; as that would cause the tooltip to not show.
 Meter=STRING
 X=0
 Y=0
 W=200
 H=50
 FontColor=0,0,0,0
 FontSize=0
 DynamicVariables=1
 Text=""
 TooltipTitle="Memory Usage"
 TooltipText="In Use: [CalcRAM:1] GB ([MeasureRAM:%,0]%)#CRLF#Available: [CalcRAMFree:1] GB#CRLF#Total: [CalcRAMTotal:1] GB"
 
[RAMGraphMax]
 Meter=Line
 MeasureName=MeasureRAM
 LineColor=#GraphLineColor#
 X=0
 Y=0
 W=200 
 H=50
 AntiAlias=1
 GraphStart=Right
 DynamicVariables=1

[RAMGraph]
 Meter=Histogram
 MeasureName=MeasureRAM
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
 MeasureName=MeasureUsedPercent
 X=190
 Y=7
 FontColor=#LabelColor#
 FontFace=Segoe UI Light
 FontSize=20
 StringAlign=Right
 StringStyle=NORMAL
 AntiAlias=1
 ;AutoScale=1
 Text="%1     "

[LabelRAM]
 Meter=STRING
 X=165
 Y=13
 FontColor=#white#,#atext#
 FontFace=Segoe UI Light
 FontSize=8
 StringAlign=Left
 StringStyle=NORMAL
 AntiAlias=0
 Text="RAM"

[LabelRAMPercentage]
 Meter=STRING
 X=165
 Y=22
 FontColor=#white#,#atext#
 FontFace=Segoe UI Light
 FontSize=10
 StringAlign=Left
 StringStyle=NORMAL
 AntiAlias=0
 Text="%"

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
 Text=""

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