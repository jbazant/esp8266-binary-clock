SKADIS_BOARD_THICKNESS = 5.1;
SKADIS_HOLE_WIDTH=4.8;
SKADIS_HOLE_LENGTH=14.8;

SKADIS_HOLES_SPACING_X=20;
SKADIS_HOLES_SPACING_Y=SKADIS_HOLES_SPACING_X * 2;

SKADIS_HOOK_HEIGHT=2*SKADIS_BOARD_THICKNESS;

module skadis_hook_single() {
    union() {
        hull() {
            cylinder(d=SKADIS_HOLE_WIDTH, h=SKADIS_BOARD_THICKNESS, $fn=20);
            translate([SKADIS_HOLE_LENGTH-SKADIS_HOLE_WIDTH, 0, 0]) 
                cylinder(d=SKADIS_HOLE_WIDTH, h=SKADIS_BOARD_THICKNESS, $fn=20);
        }
        hull() {
            translate([0, 0, SKADIS_BOARD_THICKNESS]) 
                cylinder(d=SKADIS_HOLE_WIDTH, h=SKADIS_BOARD_THICKNESS, $fn=20);
            translate([SKADIS_HOLE_WIDTH/2, 0, SKADIS_BOARD_THICKNESS]) 
                cylinder(d=SKADIS_HOLE_WIDTH, h=SKADIS_BOARD_THICKNESS, $fn=20);
        }
    }
}

module skadis_hook_double() {
    translate([0, -SKADIS_HOLES_SPACING_Y/2, 0]) skadis_hook_single();
    translate([0, SKADIS_HOLES_SPACING_Y/2, 0]) skadis_hook_single();
}

module skadis_hook_triple() {
    translate([0, SKADIS_HOLES_SPACING_Y/2, 0]) skadis_hook_single();
    translate([SKADIS_HOLES_SPACING_X, 0, 0]) skadis_hook_single();
    translate([0, -SKADIS_HOLES_SPACING_Y/2, 0]) skadis_hook_single();
}
