; TenSystem for Rainmeter - Network Widget
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

; The following measures are used to measure the amount of network traffic.
; Interface is set to Best rather than 0 (all) as measuring all interfaces includes virtual network interfaces, 
; which could return inaccurate results.

; This is used to measure the amount of both inbound (receiving) and outbound (sending) traffic, 
; for informational display on the tooltip only.
[MeasureNetTotal]
 Measure=NetTotal
 Interface=Best
 DynamicVariables=1
 ; We will be measuring in bits per second instead of bytes per second.
 UseBits=1
 ; MaxValue is not used here as this will not be used with the graphs.
 
; This and MeasureNetOut will be used for both informational display on the tooltip as well as on the graphs. 
[MeasureNetIn]
 Measure=NetIn
 Interface=Best
 DynamicVariables=1
 ; We will be measuring in bits per second instead of bytes per second.
 UseBits=1
 ; The maximum bandwidth is by default 1000000000 bits (1 gigabit),
 ; but can be configured by the user to a different value, 
 ; which is stored in the MaxBandwidth variable in the Variables.inc file.
 MaxValue=#MaxBandwidth#
 
[MeasureNetOut]
 Measure=NetOut
 Interface=Best
 DynamicVariables=1
 ; We will be measuring in bits per second instead of bytes per second.
 UseBits=1
 ; This will be the same as MeasureNetIn above to ensure both graphs are on the same scale.
 MaxValue=#MaxBandwidth#
 
[BG]
 ; This defines the size of the skin itself.
 ; The colour is defined in the variant skins themselves.
 Meter=Shape
 Shape=Rectangle 0,0,200,50 | StrokeWidth 0 | Stroke Color 0,0,0,0 | Extend BackgroundColor
 BackgroundColor=Fill Color #BGColor#

[Tooltip]
 ; The tooltip displays the following info:
 ; - outbound (sending) network traffic
 ; - inbound (receiving) network traffic
 ; - total amount of network traffic, both inbound and outbound
 ; The text meter used here is essentially made invisible but not hidden,
 ; as that would cause the tooltip to not show.
 Meter=STRING
 MeasureName=MeasureNetOut
 MeasureName2=MeasureNetIn
 MeasureName3=MeasureNetTotal 
 X=0
 Y=0
 W=200
 H=50
 FontColor=0,0,0,0
 FontSize=0
 DynamicVariables=1
 Text=""
 TooltipTitle="Network Throughput"
 TooltipText="Send: %1bps#CRLF#Receive: %2bps#CRLF#Total: %3bps"

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
 Text="NET"

; The graph for this TenSystem widget is unqiue compared to most other TenSystem widgets: 
; there will be three meters from two sources (NetIn and NetOut) displayed.
; The colours of the graphs are set in the variant skins themselves.

; For NetOut (sending), only one meter - a Line meter - will be used.
[NetOutGraph]
 Meter=Line
 MeasureName=MeasureNetOut
 LineColor=#SecondGraphLineColor#
 X=0
 Y=0
 W=200
 H=50
 AntiAlias=1
 GraphStart=Right
 DynamicVariables=1

; For NetIn (receiving), there will be two meters: a Line meter and a Histogram meter.
; The Line meter is used to visually display the peaks of the graphs, 
; while the Histogram meter is used to "fill in" the graph beneath the Line meter.
[NetInGraphMax]
 Meter=Line
 MeasureName=MeasureNetIn
 LineColor=#GraphLineColor#
 X=0
 Y=0
 W=200
 H=50
 AntiAlias=1
 GraphStart=Right
 DynamicVariables=1

[NetInGraph]
 Meter=Histogram
 MeasureName=MeasureNetIn
 PrimaryColor=#GraphFillColor#
 X=0
 Y=0
 W=200
 H=50
 AntiAlias=1
 GraphStart=Right
 DynamicVariables=1

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