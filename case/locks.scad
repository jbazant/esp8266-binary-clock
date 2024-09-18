include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>


xdistribute() {
    ydistribute() {
        union() {
            lock_cylindric();
            color([.3, .3, .3], .7) lock_cylindric_housing();
        }
        
        union() {
            lock_cylindric_small();
            color([.3, .3, .3], .7) lock_cylindric_small_housing();
        }
    }
    
    ydistribute() {
        union() {
            lock_small();
            color([.3, .3, .3], .7) lock_small_housing();
        }
        
        union() {
            lock_micro();
            color([.3, .3, .3], .7) lock_micro_housing();
        }
        
       union() {
            lock_micro_narrow();
            color([.3, .3, .3], .7) lock_micro_narrow_housing();
        }
    }
}

module lock_small() {
    upcube([1.4, 4, 3]);
    hull() {
        translate([0, 0, 2.5]) upcube([1.2, 4, 2]);
        translate([.1, 0, 3]) upcube([1.6, 4, 1]);
    }
}

module lock_small_housing() {
    translate([-.2, 0, 0]) union() {
        upcube([2, 4.2, 3]);
        hull() {
            translate([0, 0, 2.5]) upcube([2, 4.2, 2.2]);
            translate([.15, 0, 2.8]) upcube([2.3, 4.2, 1.4]);
        }
        hull() {
            upcube([2, 4.2, 1]);
            upcube([2.6, 4.8, .1]);
        }
        // for better rendering
        translate([0, 0, .3]) downcube([2.6, 4.8, 1]);
    }
}

module lock_cylindric() {
     intersection() {
        difference() {
            union() {
                translate([0, 0, 3]) 
                    cyl(l=3, d=5, chamfer=.5, align=V_TOP, $fn=20);
                cyl(l=3, d=4, align=V_TOP, $fn=20);
            }
            translate([0, 0, 1]) upcube([1.2, 5, 5]);
        }
        upcube([5, 4, 6]);
    }
}

module lock_cylindric_housing() {
    union() {
        translate([0, 0, 2.9]) 
            cyl(l=3.4, d=5.2, chamfer=.5, align=V_TOP, $fn=20);
        cyl(l=3, d=4.4, align=V_TOP, $fn=20);
        cyl(l=1, d1=5, d2=4, align=V_TOP, $fn=20);
        // for better rendering
        cyl(l=1, d=5, align=V_BOTTOM, $fn=20);
    }
}

module lock_cylindric_small() {
     intersection() {
        difference() {
            union() {
                translate([0, 0, 3.5]) 
                    cyl(l=1, d=2.8, chamfer=.3, align=V_TOP, $fn=20);
                cyl(l=3.5, d=2.2, align=V_TOP, $fn=20);
            }
            translate([0, 0, 2]) upcube([.8, 5, 5]);
        }
        upcube([2.8, 2.2, 6.5]);
    }
}

module lock_cylindric_small_housing() {
    union() {
        translate([0, 0, 3.4]) 
            cyl(l=1.4, d=3, chamfer=.3, align=V_TOP, $fn=20);
        cyl(l=3.5, d=2.4, align=V_TOP, $fn=20);
        cyl(l=1, d1=3, d2=2.4, align=V_TOP, $fn=20);
        // for better rendering
        cyl(l=1, d=3, align=V_BOTTOM, $fn=20);
    }
}

module lock_micro() {
    cuboid([1.4, 4, 1], chamfer=.3, align=V_TOP,, edges=EDGES_Z_ALL);
    translate([.1, 0, .3]) cuboid([1.6, 4, 1], chamfer=.3, align=V_TOP);
}

module lock_micro_housing() {
        cuboid([1.6, 4.1, 1.2], chamfer=.3, align=V_TOP, edges=EDGES_Z_ALL);
    translate([.1, 0, .2]) cuboid([1.7, 4.1, 1.4], chamfer=.3, align=V_TOP);
    // for better rendering
    translate([0, 0, .3]) cuboid([1.6, 4.1, 1], chamfer=.3, align=V_BOTTOM, edges=EDGES_Z_ALL);
}

module lock_micro_narrow() {
    cuboid([1.4, 2, 1], chamfer=.3, align=V_TOP,, edges=EDGES_Z_ALL);
    translate([.1, 0, .3]) cuboid([1.6, 2, 1], chamfer=.3, align=V_TOP);
}

module lock_micro_narrow_housing() {
    cuboid([1.6, 2.1, 1.2], chamfer=.3, align=V_TOP,, edges=EDGES_Z_ALL);
    translate([.1, 0, .2]) cuboid([1.7, 2.1, 1.4], chamfer=.3, align=V_TOP);
    // for better rendering
    translate([0, 0, .3]) cuboid([1.6, 2.1, 1], chamfer=.3, align=V_BOTTOM, edges=EDGES_Z_ALL);
}
