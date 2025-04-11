#!/bin/sh
# \
exec ./intiwish "$0" "$@"
###############################################
#Mise a jour de la librairie TCL :
auto_mkindex [pwd]/LIBTCL *.tcl
lappend auto_path [pwd]/LIBTCL/
# Charge le package tkimg, si present
catch {package require Img}

if {$argc != 1} {
 puts "Usage: Bayes_Classif.tcl <Colour_Image>"
 exit
} 
set img_input [lindex $argv 0]

Intinew CMD 3

label .menugeneral
menubutton .general -text "File" -menu .general.menu
set m [menu .general.menu]
$m add command -label "Reset Model" -command "reset"
$m add separator
$m add command -label "Save Model" -command "save_frame"
$m add command -label "Save Image" -command "save_image"
$m add separator
$m add command -label "Quit" -command "exit"
$m add separator
$m add command -label "About..." -command "pub"

pack .general -in .menugeneral -side left
menubutton .colour -text "Colour" -menu .colour.menu
set m [menu .colour.menu]
$m add cascade -label "Display RGB Component" -menu $m.sub1
set m1 [menu $m.sub1 -tearoff 0]
$m1 add command -label "R" -command "test_comp 1 1"
$m1 add command -label "G" -command "test_comp 1 2"
$m1 add command -label "B" -command "test_comp 1 3"
$m add separator
$m add cascade -label "Display HSV Component" -menu $m.sub2
set m2 [menu $m.sub2 -tearoff 0]
$m2 add command -label "H" -command "test_comp 2 1"
$m2 add command -label "S" -command "test_comp 2 2"
$m2 add command -label "V" -command "test_comp 2 3"
$m add separator
$m add cascade -label "Display YCrCb Component" -menu $m.sub3
set m3 [menu $m.sub3 -tearoff 0]
$m3 add command -label "Y" -command "test_comp 3 1"
$m3 add command -label "Cr" -command "test_comp 3 2"
$m3 add command -label "Cb" -command "test_comp 3 3"
pack .colour -in .menugeneral -side left
$m add separator
$m add command -label "Display Colour Image" -command "display_orig"
menubutton .classif -text "Classif" -menu .classif.menu
set m [menu .classif.menu]
$m add cascade -label "Set Space X" -menu $m.sub1
set m1 [menu $m.sub1 -tearoff 0]
$m1 add radio -label "R" -variable sp_x -value 1 -command "reinit_all"
$m1 add radio -label "G" -variable sp_x -value 2 -command "reinit_all"
$m1 add radio -label "B" -variable sp_x -value 3 -command "reinit_all"
$m1 add radio -label "H" -variable sp_x -value 4 -command "reinit_all"
$m1 add radio -label "S" -variable sp_x -value 5 -command "reinit_all"
$m1 add radio -label "V" -variable sp_x -value 6 -command "reinit_all"
$m1 add radio -label "Y" -variable sp_x -value 7 -command "reinit_all"
$m1 add radio -label "Cr" -variable sp_x -value 8 -command "reinit_all"
$m1 add radio -label "Cb" -variable sp_x -value 9 -command "reinit_all"
$m1 invoke 0
$m add separator
$m add cascade -label "Set Space Y" -menu $m.sub2
set m2 [menu $m.sub2 -tearoff 0]
$m2 add radio -label "R" -variable sp_y -value 1 -command "reinit_all"
$m2 add radio -label "G" -variable sp_y -value 2 -command "reinit_all"
$m2 add radio -label "B" -variable sp_y -value 3 -command "reinit_all"
$m2 add radio -label "H" -variable sp_y -value 4 -command "reinit_all"
$m2 add radio -label "S" -variable sp_y -value 5 -command "reinit_all"
$m2 add radio -label "V" -variable sp_y -value 6 -command "reinit_all"
$m2 add radio -label "Y" -variable sp_y -value 7 -command "reinit_all"
$m2 add radio -label "Cr" -variable sp_y -value 8 -command "reinit_all"
$m2 add radio -label "Cb" -variable sp_y -value 9 -command "reinit_all"
$m2 invoke 4
$m add separator
$m add command -label "Classify Image" -command "Go_classif"
$m add separator
$m add command -label "Classify Directory" -command "Go_classif_dir"
pack .classif -in .menugeneral -side left
pack .menugeneral -side top


image create photo image_colour -file $img_input -palette 256/256/256
Imgnew image_colour 0
image create photo image_processed -file $img_input -palette 256/256/256
Imgnew image_processed 2
canvas .img_col -width [expr [image width image_colour]] \
                -height [expr [image height image_colour]]
.img_col create image [expr [image width image_colour]/2 +1] \
                      [expr [image height image_colour]/2 +1] -image image_colour
bind .img_col <Button-1> "Designe 1 %x %y"
bind .img_col <Button-3> "Designe 2 %x %y"
bind .img_col <Motion> "DessineReticule %x %y"

set W 256
set H 256
image create photo carte_distance -width $W -height $H
Imgnew carte_distance 1
###########################################################
label .cloud -relief ridge -borderwidth 3
canvas .frame -width $W -height $H -bg white
.frame create image [expr $W/2+1] [expr $H/2+1] -image carte_distance
pack .frame -in .cloud -side top
scale .proba -from 0.05 -to 0.95 -resolution 0.05 -variable proba -orient horizontal \
             -label "Prior P(C1)" -fg blue -command "update2_distance"
pack .proba -in .cloud -side top
set proba 0.5
set distance 2
###########################################################

pack .img_col .cloud -side left

#Initialisations...
set cpt_i 0
set cpt_f 0
reinit_all
