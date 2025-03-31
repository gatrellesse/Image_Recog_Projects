proc reinit_all {} {
global list_x
global list_y
global mean_x
global mean_y
global mean_xx
global mean_xy
global mean_yy
global det
global a
global b
global c
  set list_x(1) {}
  set list_y(1) {}
  set list_x(2) {}
  set list_y(2) {}
  set mean_x(1) -1
  set mean_y(1) -1
  set mean_x(2) -1
  set mean_y(2) -1
  set det(1) 1
  set det(2) 1
  set a(1) 1
  set b(1) 1
  set c(1) 0
  set a(2) 1
  set b(2) 1
  set c(2) 0
}

proc put_point {classe x y} {
global list_x
global list_y

  if {$classe == 1} {
    .frame create line [expr $x-3] [expr $y-3] [expr $x+4] [expr $y+4] -fill blue
    .frame create line [expr $x-3] [expr $y+3] [expr $x+4] [expr $y-4] -fill blue
  } else {
    .frame create line [expr $x-3] [expr $y-3] [expr $x+4] [expr $y+4] -fill red
    .frame create line [expr $x-3] [expr $y+3] [expr $x+4] [expr $y-4] -fill red
   }
   lappend list_x($classe) $x
   lappend list_y($classe) $y
   update_frame $classe
   update_distance
}

proc update_frame {classe} {
global list_x
global list_y
global mean_x
global mean_y
global mean_xx
global mean_xy
global mean_yy
global mean_tag

if {$classe == 1} {set coul "blue"} else {set coul "red"} 
set size [llength $list_x($classe)] 
if {$size > 1} {
    if {$size > 2} {.frame delete $mean_tag($classe)}
    set newx [lindex $list_x($classe) [expr $size-1]]
    set newy [lindex $list_y($classe) [expr $size-1]]
    set mean_x($classe) [expr double(($size-1)*$mean_x($classe)+$newx)/$size]
    set mean_y($classe) [expr double(($size-1)*$mean_y($classe)+$newy)/$size]
    set mean_xx($classe) [expr double(($size-1)*$mean_xx($classe)+$newx*$newx)/$size]
    set mean_yy($classe) [expr double(($size-1)*$mean_yy($classe)+$newy*$newy)/$size]
    set mean_xy($classe) [expr double(($size-1)*$mean_xy($classe)+$newx*$newy)/$size]
    set mean_tag($classe) [eval .frame create oval [expr $mean_x($classe)-4] \
                          [expr $mean_y($classe)-4]  [expr $mean_x($classe)+4] \
                          [expr $mean_y($classe)+4] -outline $coul]
} else {
    set mean_x($classe) [lindex $list_x($classe) 0]
    set mean_y($classe) [lindex $list_y($classe) 0]
    set mean_xx($classe) [expr [lindex $list_x($classe) 0]*[lindex $list_x($classe) 0]]
    set mean_yy($classe) [expr [lindex $list_y($classe) 0]*[lindex $list_y($classe) 0]]
    set mean_xy($classe) [expr [lindex $list_x($classe) 0]*[lindex $list_y($classe) 0]]
}
}

proc update2_distance {p} {
global proba
global distance

 set proba $p
 if {$distance == 2} {update_distance}
}

proc update_distance {} {
global distance
global list_x
global list_y
global mean_x
global mean_y
global mean_xx
global mean_xy
global mean_yy
global det
global a
global b
global c
global proba

    set size(1) [llength $list_x(1)]
    set size(2) [llength $list_x(2)]
    if {$distance==0} {
	carte_distance blank
    } else {
	if {$distance==1} {
	    if {($size(1)>0)||($size(2)>0)}  {
		CMD euclide2 $mean_x(1) $mean_y(1) $mean_x(2) $mean_y(2)
	    }
	} else {
           for {set classe 1} {$classe <= 2} {incr classe} {
              if {$size($classe) > 2} {
		# calcul des matrices de covariance
		set sigma_xx($classe) [expr $mean_xx($classe) - $mean_x($classe)*$mean_x($classe)]
		set sigma_yy($classe) [expr $mean_yy($classe) - $mean_y($classe)*$mean_y($classe)]
		set sigma_xy($classe) [expr $mean_xy($classe) - $mean_x($classe)*$mean_y($classe)]
		# inversion des matrices de covariance
		set det($classe) [expr $sigma_xx($classe)*$sigma_yy($classe) - \
                                  $sigma_xy($classe)*$sigma_xy($classe)]
		set a($classe) [expr $sigma_yy($classe)/$det($classe)]
		set b($classe) [expr $sigma_xx($classe)/$det($classe)]
		set c($classe) [expr -$sigma_xy($classe)/$det($classe)]
                # La matrice C^{-1} vaut :
                # |a c|
                # |c b|
                 }
               }
             if {($size(1)>2)||($size(2)>2)}  {
	     CMD bayes_cloud $mean_x(1) $mean_y(1) $det(1) $a(1) $c(1) $b(1) \
                             $mean_x(2) $mean_y(2) $det(2) $a(2) $c(2) $b(2) $proba
                 }
	    }
	}
}

proc save_frame {} {
global cpt_f

  incr cpt_f
  set fichier model_$cpt_f.png
  image create photo monimage -format window -data .frame
  monimage write $fichier -format PNG  
} 

proc reset {} {
global list_x
global list_y
global W
global H
    
    carte_distance blank
    reinit_all
    .frame addtag croix all
    .frame delete croix
    .frame create image [expr $W/2+1] [expr $H/2+1] -image carte_distance
}
