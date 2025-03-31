#!/bin/sh
# \
exec ./intiwish "$0" "$@"
###############################################
#Mise a jour de la librairie TCL :
auto_mkindex [pwd]/LIBTCL *.tcl
lappend auto_path [pwd]/LIBTCL/
# Charge le package tkimg, si present
catch {package require Img}

set W 512
set H 512

Intinew CMD 3

label .menugeneral
menubutton .general -text "Fichier" -menu .general.menu
set m [menu .general.menu]
$m add command -label "Reset" -command "reset"
$m add command -label "Save" -command "save_frame"
$m add command -label "Quit" -command "exit"
pack .general -in .menugeneral -side left
pack .menugeneral -side top

image create photo carte_distance -width $W -height $H
Imgnew carte_distance 1

canvas .frame -width $W -height $H -bg white
.frame create image [expr $W/2+1] [expr $H/2+1] -image carte_distance
bind .frame <Button-1> "put_point 1 %x %y"
bind .frame <Button-3> "put_point 2 %x %y"
pack .frame -side top

label .action
radiobutton .b_nodist -text "none" -variable distance -value 0 -command "update_distance"
radiobutton .b_euclidean -text "Euclidean" -variable distance -value 1 -command "update_distance"
radiobutton .b_mahalanobis -text "Bayesian" -variable distance -value 2 -command "update_distance"
set distance 2
pack .b_nodist .b_euclidean .b_mahalanobis -in .action -side left
pack .action -side top

scale .proba -from 0.05 -to 0.95 -resolution 0.05 -variable proba -orient horizontal \
             -label "Prior P(C1)" -fg blue -command "update2_distance"
pack .proba -side top
set proba 0.5
set cpt_f 0

#Initialisations...
reinit_all
