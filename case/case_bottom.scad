include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>
include <skadis.scad>
use <locks.scad>

// --- global configuration ---
// (actually do not mess with it)
w=71;
l=76;
f=5;

// --- render ---
xrot(-45) case_bottom(w, l, f);

// --- modules ----
module case_bottom(w, l, f) {
    h1=8;
    h2=9;
    pcb_l=59;
    pcb_w=w-4;
    
    module custom_print_support() {
        color([.5, .5, .5], .7) union() {
            translate([w/2, 3*l/4, 0]) {
                translate([0, 5, 10.3]) rotate([45, 0, 180])         
                    right_triangle(
                        [2, 30, 30], 
                        orient=ORIENT_X, align=V_DOWN
                    );
                
                for (ty=[-l/3 : 4 : l/4]) {
                    translate([0, ty, 1]) xrot(-45) 
                        downcube([.8, .4, 2]);
                }
            }
            translate([0, l-3, 0]) xrot(45)
                translate([w/2, -15.5, .4]) cuboid(
                    [20, 30, .4], 
                    align=V_BOTTOM, 
                    fillet=10, edges=EDGES_Z_ALL
                );
            
            translate([0, l-3, 0]) xrot(45) {
                cylinder(d=12, h=.2);
                translate([w, 0, 0]) cylinder(d=12, h=.2);
            }
        }
    }
    
    module print_bad_boundary() {
        translate([w/2, l-3, 0]) xrot(45) downcube([w, l*3, 10]);
    }
    
    module pcb_stands() {
        for (tx=[4, 35, w-4]) {
            for (ty=[6, 50]) {
                translate([tx, ty, 2]) cylinder(r1=2.5, r2=2, h=h1-2-1.6);
            }
        }
    }
        
    module pcb_locks() {
        module single_lock_y() {
            translate([0, 0, h1+.1]) hull() {
                cuboid([1, 8, 1], align=V_LEFT+V_UP);
                translate([0, -2.5, 0]) 
                    right_triangle([1, 5, 1], orient=ORIENT_Y);
            }
        }
        
        module single_lock_x() {
            translate([-2.5, 0, h1+.1]) right_triangle([5, 1, 1], orient=ORIENT_X);
        }
        
        translate([2, pcb_l/4, 0]) single_lock_y();
        translate([2, 3*pcb_l/4, 0]) single_lock_y();
        translate([pcb_w+2, pcb_l/4, 0]) zrot(180) single_lock_y();
        translate([pcb_w+2, 3*pcb_l/4, 0]) zrot(180) single_lock_y();
        translate([35, pcb_l+4, 0]) zrot(180) single_lock_x();
    }
    
    module case_bottom_hull() {
        difference() {
            // base mass
            hull() {
                cuboid(
                    [w-6, l-6, 1], 
                    p1=[3, 3, 0],
                    fillet=f-2, edges=EDGES_Z_ALL
                );
                cuboid(
                    [w, l, h1+h2-3], 
                    p1=[0, 0, 3],
                    fillet=f, edges=EDGES_Z_ALL
                );
            }
            
            // base hole
            cuboid([
                pcb_w, pcb_l, h1+h2], 
                p1=[2, 4, 2],
                chamfer=3, edges=EDGES_BOTTOM-EDGE_BOT_BK );
            
            // usb hole
            cuboid([11, 10, 7], p1=[12, -1, h1-2], chamfer=1);
            
            // wemos hole
            cuboid([27.5, 35, h2], p1=[5, 2, h1+1]);
            
            // reset hole
            translate([-1, 6, h1+2]) yrot(90)
                cylinder(r=1.5, h=10, $fn=6);
            
            // fc-16 hole
            cuboid([32.4, 20, h2], p1=[10.3, l-22, h1+1]);
            
            // DHT hole
            cuboid([18, 20, h2+1], p1=[51, l-24, h1]);
        }
    }
    
    module skadis_mount() {
        translate([w/2, l-10, -10]) zrot(-90)
            skadis_hook_double();
    }
    
    module locks() {
        // translations should have same x and y as for top case
        translate([4, 2.3, h1+h2]) zrot(-90) lock_micro_narrow();
        translate([34.5, 2.3, h1+h2]) zrot(-90) lock_micro();
        translate([64, 2.3, h1+h2]) zrot(-90) lock_micro();
        translate([6, 70, h1+h2]) lock_cylindric();
        translate([47, 70, h1+h2]) lock_cylindric();
    }
    
    difference() {
        union() {
            case_bottom_hull();
            pcb_stands();
            pcb_locks();
            skadis_mount();
            custom_print_support();
            locks();
        }
        
        print_bad_boundary();
    }
}


