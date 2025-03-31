proc set_space {dimension composante} {
global sp_x
global sp_y

if {$dimension == 1} {set sp_x $composante}
if {$dimension == 2} {set sp_y $composante}
  reinit_all
}


proc DessineReticule {x y} {
global line

catch {.img_col delete $line(vert)}
catch {.img_col delete $line(horiz)}
set line(vert) [eval {.img_col create line} {$x 0 $x [image height image_colour] -fill red}]
set line(horiz) [eval {.img_col create line} {0 $y [image width image_colour] $y -fill red}]
}

proc Designe {classe x y} {
global pos_x
global pos_y
global sp_x
global sp_y


   set pos_x $x
   set pos_y $y
   puts "Classe : $classe - Position pixel : ($pos_x,$pos_y)"
   set valx [CMD getvalue $sp_x $pos_x $pos_y]
   set valy [CMD getvalue $sp_y $pos_x $pos_y]
   puts "Valeurs pixel : ($valx,$valy)"
   put_point $classe $valx $valy
}

proc Go_classif {} {
global sp_x
global sp_y

CMD test_classif $sp_x $sp_y
.img_col create image [expr [image width image_processed]/2 +1] \
                      [expr [image height image_processed]/2 +1] -image image_processed
}

proc Go_classif_dir {} {
global sp_x
global sp_y
global img_input

  set directory [file dirname $img_input]
  foreach fich [glob $directory/*] {
    image create photo image_colour -file $fich -palette 256/256/256
    Imgnew image_colour 0
    image create photo image_processed -file $fich -palette 256/256/256
    Imgnew image_processed 2
    Go_classif  
    set nom [file rootname [file tail $fich]]
    set fich_out Classif_$nom.png
    image_processed write $fich_out -format PNG
    update
   }
}

proc test_comp {space num} {

if {$space == 1} {
   CMD test_rgb $num
 } else {
   if {$space == 2} {
     CMD test_hsv $num
   } else {
     CMD test_yuv $num
     }
  }
.img_col create image [expr [image width image_processed]/2 +1] \
                      [expr [image height image_processed]/2 +1] -image image_processed
update

}
proc display_orig {} {
.img_col create image [expr [image width image_colour]/2 +1] \
                      [expr [image height image_colour]/2 +1] -image image_colour

update
}
 
proc save_image {} {
global cpt_i

  incr cpt_i
  set fichier image_$cpt_i.png
  image_processed write $fichier -format PNG  
} 

proc pub {} {

toplevel .pub
wm title .pub "Info Inti 2.0"

image create photo logo -file ./.intilogo.gif -palette 256/256/256

label .pub.img -image logo
pack .pub.img -side top
 
button .pub.button -text "Fermer" -command {set pub_vue 1}
pack .pub.button -side left -expand 1
 
grab .pub
tkwait variable pub_vue
grab release .pub
destroy .pub

}
  

