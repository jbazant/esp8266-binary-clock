
display_plate();

module display_plate() {
    module display_front() {
        difference() {
            translate([-12, -6, 0]) cube([56, 44, .6]);
            for (i=[0 : 7]) {
                for (j=[0 : 7]) {
                    if (
                        !(i == 0 && (j == 5 || j == 6))
                        && !(i < 3 && j == 2)
                        // && !(i == 1 && j == 6) // + 64 degrees is really hot
                    ) {
                        x = 2 + i * 4;
                        y = 2 + j * 4;
                        translate([x, y, -1]) cylinder(d=3.2, h=2, $fn=20);
                    }
                }
            }
        }
    }
    
    module frame(i, j, w, h=1) {
        x = -.4 + i * 4;
        y = -.4 + j * 4;
        color("blue" ) translate([x, y, .4]) difference() {
            cube([w*4+.8, h*4+.8, .6]);
            translate([.8, .8, -1]) cube([w*4-.8, h*4-.8, 2]);
        }
    }
    
    module frames() {
        frame(2, -1, 6);
        frame(-2.5, -1, 4.5, 3);
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
        color("red") translate([x, y, .6]) linear_extrude(.4) 
            text(str, size=2.5, halign=halign, valign="center");
    }
    
    module labels() {
        bottomLabels=["?", "x", "32", "16", "8", "4", "2", "1"];
        topLabels=["-", "64", "32", "16", "8", "4", "2", "1"];
        leftLabels=["NTP", "DHT", " ", "dow", "moon", "year", "RH", "temp"];
        rightLabels=["sec", "min", "hour", "day", "mon", "%100", "%", "°C"]; 
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

    module screw_hole(x, y, delta) {
        translate([x, y, 0]) cylinder(d=3+delta, h=1, $fn=20);
    }
    
    module screw_holes(delta=0) {
        union() {
            screw_hole(-8, -2, delta);
            screw_hole(-8, 34, delta);
            screw_hole(40, -2, delta);
            screw_hole(40, 34, delta);
        }
    }
    
    difference() {
        union() {
            display_front();
            frames();
            labels();
            screw_holes(1.6);
        }
        screw_holes();
    }
}

