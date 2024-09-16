include <BOSL/constants.scad>
use <BOSL/transforms.scad>
use <BOSL/shapes.scad>
use <locks.scad>

// --- global configuration ---
// (actually do not mess with it)
f=5;

// --- render ---
display_plate();

// --- modules ----
module display_plate(f=5, lables_font="Roboto:style=Bold") {
    module display_front() {
        difference() {
            color("white") hull() {
                cuboid(
                    [53, 41, .2], 
                    p1=[-10.5, -4.5, 0],
                    edges=EDGES_Z_ALL, 
                    fillet=f 
                );
                cuboid(
                    [51, 39, 1.2], 
                    p1=[-9.5, -3.5, 0],
                    edges=EDGES_Z_ALL, 
                    fillet=f-1 
                );
            }
            for (i=[0 : 7]) {
                for (j=[0 : 7]) {
                    if (
                        !(i == 0 && (j == 5 || j == 6))
                        && !(i < 3 && j == 2)
                    ) {
                        x = 2 + i * 4;
                        y = 2 + j * 4;
                        translate([x, y, -1]) cylinder(d=3.2, h=3, $fn=20);
                    }
                }
            }
        }
    }
    
    module display_locks() {
        // translations should have same x and y as for top case
        translate([5, 3, 0]) zrot(-90) lock_micro_housing();
        translate([48, 3, 0]) zrot(-90) lock_micro_housing();
        translate([5, 38, 0]) zrot(90) lock_micro_housing();
        translate([48, 38, 0]) zrot(90) lock_micro_housing();
    }
    
    module frame(i, j, w, h=1, edges=EDGES_Z_ALL) {
        x = -.4 + i * 4;
        y = -.4 + j * 4;

        color("red") translate([x, y, 0]) difference() {
            cuboid(
                [w*4+.8, h*4+.8, 1.6], 
                p1=[0, 0, 0] 
            );
            cuboid(
                [w*4-.8, h*4-.8, 3], 
                p1=[.8, .8, -1], 
                fillet=1.6, edges=edges,
                $fn=20
            );
        }
    }

    module status_frame() {
        difference() {
            union() {
                frame(-2.5, 0, 4.5, 2, edges=EDGES_Z_ALL-EDGE_FR_RT);
                frame(0, -1, 2, 3, edges=EDGES_Z_ALL-EDGE_BK_LF);
            }
            cuboid([2, 7.2, 3], p1=[-1, .4, -1]);
            cuboid([7.2, 2, 3], p1=[.4, -1, -1]);
        }
    }
        
    module frames() {
        status_frame();
        frame(2, -1, 6, edges=EDGE_FR_RT+EDGE_BK_RT);
        frame(2, 0, 8.5);
        frame(2, 1, 8.5);
        frame(3, 2, 7.5);
        frame(-2.5, 2, 5.5, 2);
        frame(3, 3, 7.5);
        frame(-2.5, 4, 6.5);
        frame(4, 4, 6.5);
        frame(-2.5, 5, 13);
        frame(-2.5, 6, 13);
        frame(-2.5, 7, 13);
        frame(0, 8, 8);
    }
    
    module label(str, i, j, halign="center") {
        x = 2 + i * 4;
        y = 2 + j * 4;
        color("red") translate([x, y, 1.2]) linear_extrude(.4) 
            text(
                str, size=2.5, 
                halign=halign, valign="center", 
                font=lables_font
            );
    }
    
    module labels() {
        bottomLabels=["F", "E", "32", "16", "8", "4", "2", "1"];
        topLabels=["_", "64", "32", "16", "8", "4", "2", "1"];
        leftLabels=["NTP", "DHT", " ", "DOW", "LUN", "YEAR", "RH", "TMP"];
        rightLabels=["SEC", "MIN", "HR", "DAY", "MON", "%100", "%", "°C"]; 
        dowLabels=["4", "2", "1"];
        
        for (i=[0 : 7]) {
            label(bottomLabels[i], i, -1);
            label(topLabels[i], i, 8);
            label(leftLabels[i], -.6, i, "right");
            label(rightLabels[i], 7.6, i, "left");
        }
        
        for (i=[0 : 2]) {
            label(dowLabels[i], i, 2);
        }
    }
    
    difference() {
        translate([10.5, 4.5, 0]) {
            display_front();
            frames();
            labels();
        }
        display_locks();
    }
}