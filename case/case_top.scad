include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>
use <locks.scad>

// --- global configuration ---
// (actually do not mess with it)
w=71;
l=76;
f=5;

// --- render ---
case_top(w, l, f);

// --- modules ----
module case_top(w, l, f) {
    module display_housing() {
        module display_walls() {
            translate([10.5, 4.5, 0]) difference() {
                cuboid(
                    [53, 41, 6.8], 
                    p1=[-10.5, -4.5, 0],
                    edges=EDGES_Z_ALL, 
                    fillet=f 
                );
                translate([-.2, -.2, -1]) cube([32.4, 32.4, 10]);
            }
        }
        
        module display_locks() {
            translate([0, 0, 6.8]) {
                // translations should have same x and y as for "display plate"
                translate([5, 3, 0]) zrot(-90) lock_micro();
                translate([48, 3, 0]) zrot(-90) lock_micro();
                translate([5, 38, 0]) zrot(90) lock_micro();
                translate([48, 38, 0]) zrot(90) lock_micro();
            }
        }
        
        union() {
            display_walls();
            display_locks();
        }
    }
        
    module top_plate() {
        module grid(x, y, w, l) {
            hull() {
                translate([x, y, 1]) cube([w, l, 2]);
                translate([x-1, y-1, 2]) cube([w+2, l+2, 1]);
            }
            for (tx=[x : 3 : x+w-1]) {
                for (ty=[y : 3 : y+l-1]) {
                    translate([tx, ty, -1]) cube([2.2, 2.2, 4]);
                }
            }
        }
        
        module dht_grid() {
            grid(54.5, 43.5, 11.2, 17.2);
        }
        
        module wemos_grid() {
            grid(14.5, 13.5, 8.2, 14.2);
        }
        
        difference() {
            hull() {
                cuboid(
                    [w, l, 1], p1=[0, 0, 0],
                    edges=EDGES_Z_ALL, fillet=f 
                );
                cuboid(
                    [w-2, l-2, 2], p1=[1, 1, 0],
                    edges=EDGES_Z_ALL, fillet=f-1
                );
            }
            
            // display housing (difference)
            cuboid(
                [53, 41, 10], p1=[0, 35, -3],
                edges=EDGES_Z_ALL, fillet=f 
            );
            
            dht_grid();
            wemos_grid();
        }  
    }

    module button_hole() {
        cylinder(r=6, h=20, $fn=40);
    }
    
    module button_holes() {
        translate([41, 30, -3]) button_hole();
        translate([59.7, 30, -3]) button_hole();
    }
    
    module locks() {
        // translations should have same x and y as for bottom case
        translate([4, 2.3, 0]) zrot(-90) lock_micro_narrow_housing();
        translate([34.5, 2.3, 0]) zrot(-90) lock_micro_housing();
        translate([64, 2.3, 0]) zrot(-90) lock_micro_housing();
        translate([6, 70, 0]) lock_cylindric_housing();
        translate([47, 70, 0]) lock_cylindric_housing();
    }
    
    difference() {
        union() {
            top_plate();
            translate([0, 35, 0]) display_housing();
        }
        button_holes();
        color("red") locks();
    }
}

